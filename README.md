# Semantic-Informed Driver Fuzzing Without Both the Hardware Devices and the Emulators

Device drivers are security-critical. In monolithic kernels like Linux, there are hundreds of thousands of drivers which run in the same privilege as the core kernel. Consequently, a bug in a driver can compromise the whole system. More critically, drivers are particularly buggy. Existing studies show that drivers tend to have a higher bug density and have become a major security threat. Existing testing techniques have to focus the fuzzing on a limited number of drivers that have the corresponding devices or the emulators, thus cannot scale.

The tool implements a semantic-informed mechanism that efficiently generates inputs to properly construct relevant data structures to pass the “validation chain” in driving initialization, which enables subsequent device-free driver fuzzing. More details can be found in the paper shown at the bottom.

## How to use 

### Build LLVM 
```sh 
	$ cd llvm 
	$ ./build-llvm.sh 
	# The installed LLVM is of version 10.0.0 
```

### Build the Semantic-informed analyzer 
```sh 
	# Build the analysis pass
	$ cd ../analyzer 
	$ make 
	# Now, you can find the executable, `kanalyzer`, in `build/lib/`
```
 
### Prepare LLVM bitcode files of OS kernels

+ Config the kernel to enable all device drivers by modify the `.config` file
+ Install Bear tools to collect the compile commands
	`bear make`
+ Use `python-linux-bc.py` to generate `command.sh`
+ Generate all IR files

### Run the  analyzer
```sh
	# To analyze a single bitcode file, say "test.bc", run:
	$ ./build/lib/kanalyzer -sc test.bc
	# To analyze a list of bitcode files, put the absolute paths of the bitcode files in a file, say "bc.list", then run:
	$ ./build/lib/kalalyzer -mc @bc.list
```

## More details
```sh
@inproceedings{drfuzz-ndss22,
  title        = {{Semantic-Informed Driver Fuzzing Without Both the Hardware Devices and the Emulators}},
  author       = {Wenjia Zhao and Kangjie Lu and Qiushi Wu and Yong Qi},
  booktitle    = {Proceedings of the Network and Distributed Systems Security (NDSS) Symposium 2022},
  month        = April,
  year         = 2022,
  address      = {San Diego, CA},
}
```
