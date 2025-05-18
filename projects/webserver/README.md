开发环境 MacOS下的Ubuntu22.04 Docker Images, 架构arm64, 采用micromamba虚拟环境.

方法:

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
之后回到宿主机, ssh链接

```shell
ssh root@localhost -p 2222
```

注意之后如果失败了可以检测一下这个container是否在running
```shell
docker ps -a | grep my-ubuntu
```
如果显示的是Existed, 就需要`docker start my-ubuntu`启动一下

成功ssh之后下载micromamba

```shell
curl -Ls https://micro.mamba.pm/api/micromamba/linux-aarch64/latest | tar -xvj bin/micromamba
./bin/micromamba shell init -s bash -r ~/micromamba
source ~/.bashrc

micromamba create -n clang-env clang clangxx -c conda-forge
micromamba activate clang-env
```
