# 1. Abstract

webserver is a high-performance server based on C++, built with CMake.

# 2. Development Environment

Ubuntu 22.04 Docker Image on MacOS, arm64 architecture, using micromamba virtual environment.

Steps:

```shell
docker pull ubuntu:22.04
docker run -d -p 2222:22 --name my-ubuntu ubuntu:22.04
docker exec -it my-ubuntu /bin/bash
```

```shell
apt update && apt install -y openssh-server
echo "PermitRootLogin yes" >> /etc/ssh/sshd_config
echo "root:123456" | chpasswd
mkdir /var/run/sshd
/usr/sbin/sshd -D &
```
Then return to the host machine and connect via SSH

```shell
ssh root@localhost -p 2222
```

Note: If it fails later, you can check if this container is running


```shell
docker ps -a | grep my-ubuntu
```
If it shows "Existed", you need to run `docker start my-ubuntu` to start it

After successfully SSH-ing, download micromamba

```shell
curl -Ls https://micro.mamba.pm/api/micromamba/linux-aarch64/latest | tar -xvj bin/micromamba
./bin/micromamba shell init -s bash -r ~/micromamba
source ~/.bashrc

micromamba create -n clang-env clang clangxx -c conda-forge
micromamba activate clang-env
```

# 3. Docs
