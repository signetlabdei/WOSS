# New NS-Miracle classes

WOSS introduces a new Packet header struct hdr_woss, WossChannelModule, WossMPropagation and WossMInterferenceMIV classes for channel computations and interference calculations.

It also supports a ChannelEstimator, ChEstimatorPlugIn, ClMsgChannelEstimation for channel estimation purposes.

It adds support for positioning with WossPosition and for waypoint mobility with WossWpPosition.

Finally it provides the WossMPhyBpsk class for BPSK modulation with power control based on channel estimations.