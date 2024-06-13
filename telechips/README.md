# For telechips boards build scripts


### Guide

1. 커스텀 커맨드 경로 설정
```
$ echo "export PATH=$HOME/.usr/bin:$PATH" >> ${HOME}/.profile
```
```
$ mkdir -p ${HOME}/.usr/bin
$ mkdir -p ${HOME}/.usr/share
```
<br>

2. kernel-script 다운로드
```
$ git clone https://github.com/how2flow/kernel-scripts -b telechips
$ cp kernel-scripts/telechips/kernel-script ${HOME}/.usr/bin/kernel-script
$ cp kernel-scripts/telechips/scripts/* ${HOME}/.usr/share/kernel-scripts/
```
<br>

3. boot-firmware 세팅

```
$ cd ${HOME}
$ git clone {boot-firmware repo} -b "타겟 보드(tcc807x / ..)"
$ cp kernel-scripts/telechips/boot-firmware/mksd.sh ${HOME}/boot-firmware
```
<br>

4. ps1 파일 설정
```
kernel-scripts/telechips/fwdn/file-tree 참조
```
<br>

5. uboot / kernel 파일 트리
<br>

u-boot
```
~/u-boot/u-boot-core$ tree -L 1
├── api
├── arch
├── board
├── boot
...
```
<br>

kernel
```
~/kernel$ tree -L 1
.
├── initramfs64.cpio.lzo
├── initramfs.cpio.lzo
├── kernel-5.10
               `initramfs64.cpio.lzo -> ../initramfs64.cpio.lzo
...
```
<br>
