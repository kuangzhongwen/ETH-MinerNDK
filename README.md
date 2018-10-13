# eth-gpu-miner-ndk
Eth gpu miner ndk with OpenCL on android

Fork from https://github.com/ethereum-mining/ethminer.git

Adapt Android OpenCL, compile with ndk.

#### Precautions  
      1. Before the migration, you need to compile the boost with the cross-compilation platform. Note that you need to adapt to different ABI platforms.  
      2. You need to convert .cl to a .h binary based on bin2h.cmake.  
      3. The source code execution logic entry: ../ethminer-master/ethminer/MinerAux.h.
     
#### Claim  
      1. The OpenCL version must be 1.2 and above, and the kernel internal code is required.  
      2. OpenCL's Global Mem Size must be >=2558525056, the current demand for eth's dag size.  
      3. CL device type: CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_ACCELERATOR
