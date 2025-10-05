/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */ 


/**
 * @file   thread-pool-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions of woss::ThreadPool class
 *
 * Definitions of woss::ThreadPool class
 */

#ifndef THREADPOOL_DEFINITIONS_H
#define THREADPOOL_DEFINITIONS_H

#ifdef WOSS_MULTITHREAD

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

namespace woss {

  /**
   * ThreadPool class manages a fixed number of worker threads
   * that pick up and execute tasks from a shared queue.
  */
  class ThreadPool {

    public:

    /**
     * Task type: A generic callable that takes no arguments and returns nothing.
     */
    using Task = std::function< void() >;

    /**
     * Constructor, Initializes the thread pool with a specified number of threads.
     * 
     * @param n_threads number of threads. If set to 0, then system will automatically pick the max allowed number of threads.
     */
    explicit ThreadPool(size_t n_threads)
      : num_threads(n_threads),
        workers(),
        tasks(),
        queue_mutex(),
        task_cond_var(),
        stop_flag(false)
      {
        if (num_threads == 0) {
          num_threads = std::thread::hardware_concurrency();
        }

        start();
      }

    /**
     * Destructor, Ensures all worker threads are joined and the pool is properly shut down.
     */
    ~ThreadPool() {
      stop();
    }

    /**
     * @brief Submits a task to the thread pool.
     * 
     * @param f is the function type (e.g., a lambda, a regular function pointer, std::function).
     * @param args are the types of the arguments for the function F.
     * @returns return type is automatically deduced to be a std::future wrapping the return type of the function F.
     */
    template <class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
      // Deduce the return type of the function f when called with its arguments.
      using return_type = decltype(std::forward<F>(f)(std::forward<Args>(args)...));

      // Create a std::packaged_task. This object "packages" the function and its arguments,
      // making it executable and providing a std::future for its result.
      // std::bind creates a callable object that binds the arguments to the function,
      // resulting in a callable that takes no arguments.
      auto task = std::make_shared< std::packaged_task< return_type() > > (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
      );

      // Get a future from the packaged_task. This future will be used to retrieve the result.
      std::future<return_type> res = task->get_future();
      
      // Check if the pool has already been stopped. If it has, it will return the future 
      // which won't be run in the thread pool.
      if (stop_flag == true) {
        return res;
      }

      {
        std::unique_lock<std::mutex> lock(queue_mutex);
        // Add the task to the queue. The task stored in the queue is a lambda
        // that simply invokes the packaged_task. This lambda conforms to Task = std::function<void()>.
        tasks.emplace([task]() {
            (*task)(); // Execute the packaged_task. This will set the result in the associated future.
        });
      }

      // Notify one waiting thread that there is a new task to process.
      task_cond_var.notify_one();
      return res;
    }

    /**
     * @brief Gets the number of threads in the pool
     * 
     * @returns the number of std::thread created in the pool
     */
    size_t getNumThreads() {
      return num_threads; 
    }

    private:
    /**
     * The number of threads in the pool.
     */
    size_t num_threads;
    /**
     * The collection of worker threads in the pool.
     */
    std::vector<std::thread> workers;
    /**
     * Queue of tasks to be executed.
     */
    std::queue<Task> tasks;
    /**
     * Mutex to protect access to the tasks queue.
     */
    std::mutex queue_mutex;
    /**
     * Condition variable for task synchronization.
     */
    std::condition_variable task_cond_var;
    /**
     * Flag to signal threads to terminate.
     */
    bool stop_flag;

    /**
     * Creates up to num_threads std::thread and make them wait for work
     */
    void start() {
      // Start the worker threads.
      for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back( [this] {
          // Each worker thread runs an infinite loop to process tasks.
          while (stop_flag == false) {
            Task task;

            { // lock scope
              std::unique_lock<std::mutex> lock(queue_mutex);

              // Wait until there are tasks in the queue OR the pool is stopped.
              // The lambda predicate prevents spurious wakeups.
              task_cond_var.wait(lock, [this] {
                  return ((tasks.empty() == false) || (stop_flag == true));
              });

              // Exit condition: If the pool is stopped,
              // this thread can terminate gracefully.
              if (stop_flag == true) {
                  return;
              }
              // Retrieve the task from the front of the queue.
              task = std::move(tasks.front());
              tasks.pop();
            }
            // Execute the task outside the mutex lock.
            // This is crucial to avoid blocking the queue while a task is running,
            // allowing other threads to submit or retrieve tasks.
            task();
          }
        });
      }
    }

    /**
     * Sets the stop_flag, wakes up every thread and wait for them to exit.
     */
    void stop() {
      {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
      }
      // Wake up all waiting threads so they can check the 'stop_flag' flag
      // and terminate gracefully.
      task_cond_var.notify_all();
      // Join all worker threads. join() blocks until the thread has finished its execution.
      for (std::thread& worker : workers) {
          worker.join();
      }
    }

  };

}

#endif // WOSS_MULTITHREAD

#endif // THREADPOOL_DEFINITIONS_H