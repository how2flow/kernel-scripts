#!/bin/bash

# This script must exist in root-path of 'boot-firmware/'
# written by Steve Jeong (steve@how2flow.net)

#set -x

CHIP=("tcc807x")
K_VER="5.10"
KERNEL="${HOME}/kernel/kernel-${K_VER}/"
UBOOT="${HOME}/u-boot/u-boot-core/"

cherrypick() {
  local cherry

  cherry=$(find ${1} -name ${2})
  if [ -z ${cherry} ]; then
    cherry=$(find ${1} -name ${2} | grep ${3})
  fi

  echo ${cherry}
}

cherrynpick() {
  local cherry

  cherry=$(find ${1} -name ${2} | grep -v ${3})
  if [ -z ${cherry} ]; then
    cherry=$(find ${1} -name ${2})
  fi

  echo ${cherry}
}

create_gpt_partition_list() {
  local target

  target=${1}

  case ${target} in
    tcc807x)
      cat << __EOF > tools/mktcimg/gpt_partition.list
bl3_ap0_a:2048k@../ap0_bl3.rom
bl3_ap0_b:2048k@../ap0_bl3.rom
bl3_ap1_a:2048k@../ap1_bl3.rom
bl3_ap1_b:2048k@../ap1_bl3.rom
boot:40960k@../boot.img
dtb:1024k@../tcc8070-lpd4x322.dtb
misc:1024k@
env:16k@
subcore_boot:40960k@../boot_sub.img
subcore_dtb:1024k@../tcc8070-subcore-lpd4x322.dtb
subcore_misc:1024k@
subcore_env:16k@
data:0k@
__EOF
sleep .5
      ;;
  esac
}

create_symlink() {
  local data
  local dtb_main
  local dtb_sub
  local idx
  local kernel_main
  local kernel_sub
  local line
  local path
  local root_path
  local target
  local uboot

  data=$(cat tools/mktcimg/gpt_partition.list)
  root_path=$(pwd -P)
  cd tools/mktcimg/

  for line in ${data}; do
    idx=${line%%:*}
    path=${line#*@}
    target=${line#*/}

    if [ -z ${path} ]; then
      continue
    fi

    # u-boot: main is different with sub
    if [ $(echo ${idx} | grep "bl" | wc -l) -gt 0 ]; then
      uboot=$(cherrypick ${UBOOT} ${target} "build")
      [ ! -f ${uboot} ] && echo "There is no ${target} in ${UBOOT}"
      rm -rf ${path} && ln -s ${uboot} ${path}
    # kernel: main is equal with sub
    elif [ $(echo ${idx} | grep "boot" | wc -l) -gt 0 ]; then
      # kernel-sub
      if [ $(echo ${idx} | grep "sub" | wc -l) -eq 1 ]; then
        kernel_sub=$(cherrypick ${KERNEL} ${target} "build_sub")
        if [ -z ${kernel_sub} ]; then
        kernel_sub=$(cherrypick ${KERNEL} ${target/_sub/} "build_sub")
        mv ${kernel_sub} ${kernel_sub/.img/_sub.img}
        kernel_sub=${kernel_sub/.img/_sub.img}
        fi
        [ ! -f ${kernel_sub} ] && echo "There is no ${target} in ${KERNEL}"
        rm -rf ${path} && ln -s ${kernel_sub} ${path}
      # kernel-main
      else
        kernel_main=$(cherrynpick ${KERNEL} ${target} "build_sub")
        [ ! -f ${kernel_main} ] && echo "There is no ${target} in ${KERNEL}"
        rm -rf ${path} && ln -s ${kernel_main} ${path}
      fi
    # dtb: main is equal with sub
    elif [ $(echo ${idx} | grep "dtb" | wc -l) -gt 0 ]; then
      # dtb-sub
      if [ $(echo ${idx} | grep "sub" | wc -l) -eq 1 ]; then
        dtb_sub=$(cherrypick ${KERNEL} ${target/_sub/} "build_sub")
        [ ! -f ${dtb_sub} ] && echo "There is no ${target} in ${KERNEL}"
        rm -rf ${path} && ln -s ${dtb_sub} ${path}
      # dtb-main
      else
        dtb_main=$(cherrynpick ${KERNEL} ${target} "build_sub")
        [ ! -f ${dtb_main} ] && echo "There is no ${target} in ${KERNEL}" 
        rm -rf ${path} && ln -s ${dtb_main} ${path}
      fi
    fi

  done

  cd ${root_path}
}

make_sd() {
  local root_path
  local target

  root_path=$(pwd -P)
  target=${1}

  cd tools/mktcimg/
  case ${target} in
    ufs) # ufs
      ./mktcimg --parttype gpt --storage_size 63988301824 --fplist gpt_partition.list --outfile SD_Data.fai --area_name "SD Data" --gptfile SD_Data.gpt --sector_size 4096
      mv SD_Data* ${root_path}
      ;;
    *) # mmc
      ./mktcimg --parttype gpt --storage_size 31268536320 --fplist gpt_partition.list --outfile SD_Data.fai --area_name "SD Data" --gptfile SD_Data.gpt
      mv SD_Data* ${root_path}
      ;;
  esac

  cd ${root_path}
}

echo "=== Select Chip model ==="
for ((i=0; i<${CHIP[@]}+1; i++)); do
  echo "[${i+1}] ${CHIP[$i]}"
done

read -r -p "CHIP: " opt

create_gpt_partition_list ${CHIP[$(expr ${opt} - 1)]}
create_symlink
sleep 3
make_sd
