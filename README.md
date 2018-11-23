# blazingdb-io

# testing-libgdf

## Dependencies

### Runtime dependencies

#### AWS C++ SDK:

- For Debian/Ubuntu based systems:

```shell-script
sudo apt-get install libcurl3 libssl1.0.0 zlib1g libuuid1
```

- For Red Hat/Fedora based systems:

```shell-script
sudo dnf install libcurl openssl libuuid
```

You can check more details about the AWS C++ SDK prerequisites here:

http://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html


### Build dependencies

#### 1.- AWS C++ SDK (version >= 1.0.51: hash 864eb0bca8b48427f94850b7a8311ef0ae0f433b)
https://aws.amazon.com/documentation/sdk-for-cpp/

First, you need to install the prerequisites:

- For Debian/Ubuntu based systems:

```shell-script
sudo apt-get install libcurl4-openssl-dev libssl-dev uuid-dev zlib1g-dev
```

- For Red Hat/Fedora based systems:

```shell-script
sudo dnf install libcurl-devel openssl-devel libuuid-devel
```

You can check more details about the AWS C++ SDK prerequisites here:

http://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html

Finally, we need to build all the AWS C++ SDK libraries as static libs (.a files):

```shell-script
cd /some-directory/
git clone https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp/
git checkout 864eb0bca8b48427f94850b7a8311ef0ae0f433b
mkdir build
cd build/
cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=OFF -DENABLE_UNITY_BUILD=ON -DMINIMIZE_SIZE=ON -DCUSTOM_MEMORY_MANAGEMENT=0 -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

Note: 
- We are using the revision: 864eb0bca8b48427f94850b7a8311ef0ae0f433b
- Make sure you run cmake for AWS C++ SDK with -DCUSTOM_MEMORY_MANAGEMENT=0 as listed above.
 

# Building

```bash
mkdir build
cd build
cmake -Daws-sdk-cpp_DIR:PATH=/some-directory/aws-sdk-cpp/build/ .. 
make -j8
```
