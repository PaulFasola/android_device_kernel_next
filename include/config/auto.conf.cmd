deps_config := \
	lib/xz/Kconfig \
	lib/Kconfig \
	drivers/lguest/Kconfig \
	virt/kvm/Kconfig \
	arch/x86/kvm/Kconfig \
	crypto/asymmetric_keys/Kconfig \
	drivers/crypto/dx_ccp63/Kconfig \
	drivers/crypto/dx_ccp441/Kconfig \
	drivers/crypto/ux500/Kconfig \
	drivers/crypto/nx/Kconfig \
	drivers/crypto/caam/Kconfig \
	drivers/crypto/Kconfig \
	crypto/async_tx/Kconfig \
	crypto/Kconfig \
	security/integrity/evm/Kconfig \
	security/integrity/ima/Kconfig \
	security/integrity/Kconfig \
	security/yama/Kconfig \
	security/apparmor/Kconfig \
	security/tomoyo/Kconfig \
	security/smack/Kconfig \
	security/selinux/Kconfig \
	security/keys/Kconfig \
	security/Kconfig \
	lib/Kconfig.kmemcheck \
	lib/Kconfig.kgdb \
	samples/Kconfig \
	kernel/trace/Kconfig \
	mm/Kconfig.debug \
	lib/Kconfig.debug \
	arch/x86/Kconfig.debug \
	fs/dlm/Kconfig \
	fs/nls/Kconfig \
	fs/9p/Kconfig \
	fs/afs/Kconfig \
	fs/coda/Kconfig \
	fs/ncpfs/Kconfig \
	fs/cifs/Kconfig \
	fs/ceph/Kconfig \
	net/sunrpc/Kconfig \
	fs/nfsd/Kconfig \
	fs/nfs/Kconfig \
	fs/exofs/Kconfig.ore \
	fs/efivarfs/Kconfig \
	fs/f2fs/Kconfig \
	fs/exofs/Kconfig \
	fs/ufs/Kconfig \
	fs/sysv/Kconfig \
	fs/pstore/Kconfig \
	fs/romfs/Kconfig \
	fs/qnx6/Kconfig \
	fs/qnx4/Kconfig \
	fs/hpfs/Kconfig \
	fs/omfs/Kconfig \
	fs/minix/Kconfig \
	fs/freevxfs/Kconfig \
	fs/squashfs/Kconfig \
	fs/cramfs/Kconfig \
	fs/logfs/Kconfig \
	fs/ubifs/Kconfig \
	fs/jffs2/Kconfig \
	fs/efs/Kconfig \
	fs/bfs/Kconfig \
	fs/befs/Kconfig \
	fs/hfsplus/Kconfig \
	fs/hfs/Kconfig \
	fs/ecryptfs/Kconfig \
	fs/affs/Kconfig \
	fs/adfs/Kconfig \
	fs/configfs/Kconfig \
	fs/sysfs/Kconfig \
	fs/proc/Kconfig \
	fs/ntfs/Kconfig \
	fs/exfat/Kconfig \
	fs/fat/Kconfig \
	fs/udf/Kconfig \
	fs/isofs/Kconfig \
	fs/cachefiles/Kconfig \
	fs/fscache/Kconfig \
	fs/sdcardfs/Kconfig \
	fs/fuse/Kconfig \
	fs/autofs4/Kconfig \
	fs/quota/Kconfig \
	fs/notify/fanotify/Kconfig \
	fs/notify/inotify/Kconfig \
	fs/notify/dnotify/Kconfig \
	fs/notify/Kconfig \
	fs/nilfs2/Kconfig \
	fs/btrfs/Kconfig \
	fs/ocfs2/Kconfig \
	fs/gfs2/Kconfig \
	fs/xfs/Kconfig \
	fs/jfs/Kconfig \
	fs/reiserfs/Kconfig \
	fs/jbd2/Kconfig \
	fs/jbd/Kconfig \
	fs/ext4/Kconfig \
	fs/ext3/Kconfig \
	fs/ext2/Kconfig \
	fs/Kconfig \
	drivers/firmware/efi/Kconfig \
	drivers/firmware/google/Kconfig \
	drivers/firmware/Kconfig \
	drivers/contexthub/Kconfig \
	drivers/connectivity/hisi/wifi/Kconfig \
	drivers/connectivity/hisi/platform/Kconfig \
	drivers/connectivity/hisi/Kconfig \
	drivers/connectivity/Kconfig \
	drivers/hwtc/Kconfig \
	drivers/dsm/Kconfig \
	drivers/hisi_pilot/Kconfig \
	drivers/hwusb/hw_gadget/Kconfig \
	drivers/hwusb/Kconfig \
	drivers/huawei_platform_legacy/Kconfig \
	drivers/huawei_platform/power/usb_short_circuit_protect/Kconfig \
	drivers/huawei_platform/power/msgnotify/Kconfig \
	drivers/huawei_platform/power/smpl/Kconfig \
	drivers/huawei_platform/power/bastet/Kconfig \
	drivers/huawei_platform/power/ate/Kconfig \
	drivers/huawei_platform/power/hw_kstate/Kconfig \
	drivers/huawei_platform/power/battery/Kconfig \
	drivers/huawei_platform/power/coulometer/hisi_coul/Kconfig \
	drivers/huawei_platform/power/charger/Kconfig \
	drivers/huawei_platform/power/Kconfig \
	drivers/huawei_platform/antenna_fpga/Kconfig \
	drivers/huawei_platform/audio/invert_hs/Kconfig \
	drivers/huawei_platform/audio/earpiece_pa/Kconfig \
	drivers/huawei_platform/audio/tfa9895/Kconfig \
	drivers/huawei_platform/audio/anc_hs/Kconfig \
	drivers/huawei_platform/audio/speakerID/Kconfig \
	drivers/huawei_platform/audio/max98925/Kconfig \
	drivers/huawei_platform/audio/Kconfig \
	drivers/huawei_platform/chr/Kconfig \
	drivers/huawei_platform/chipverification/Kconfig \
	drivers/huawei_platform/storage/ddrinfo/Kconfig \
	drivers/huawei_platform/storage/Kconfig \
	drivers/huawei_platform/ext_modem/Kconfig \
	drivers/huawei_platform/usb/typec/fusb301/Kconfig \
	drivers/huawei_platform/usb/typec/tusb320/Kconfig \
	drivers/huawei_platform/usb/typec/pericom30216/Kconfig \
	drivers/huawei_platform/usb/typec/Kconfig \
	drivers/huawei_platform/usb/otg_gpio_switch/Kconfig \
	drivers/huawei_platform/usb/switch/Kconfig \
	drivers/huawei_platform/usb/Kconfig \
	drivers/huawei_platform/nfc/nfc_bcm20793/Kconfig \
	drivers/huawei_platform/nfc/NXP_PN547_64/Kconfig \
	drivers/huawei_platform/nfc/Kconfig \
	drivers/huawei_platform/log/boot_log/Kconfig \
	drivers/huawei_platform/log/goldeneye/Kconfig \
	drivers/huawei_platform/log/Kconfig \
	drivers/huawei_platform/irda/Kconfig \
	drivers/huawei_platform/touthscreen/panel/cyttsp5/Kconfig \
	drivers/huawei_platform/touthscreen/panel/Kconfig \
	drivers/huawei_platform/touthscreen/Kconfig \
	drivers/huawei_platform/inputhub/iom3/Kconfig \
	drivers/huawei_platform/inputhub/iom7/Kconfig \
	drivers/huawei_platform/sensor/fingerprint/Kconfig \
	drivers/huawei_platform/sensor/hall_inputhub/Kconfig \
	drivers/huawei_platform/sensor/hall/Kconfig \
	drivers/huawei_platform/sensor/compass/Kconfig \
	drivers/huawei_platform/sensor/light/Kconfig \
	drivers/huawei_platform/sensor/accelerometer/Kconfig \
	drivers/huawei_platform/sensor/Kconfig \
	drivers/huawei_platform/connectivity/hw_connectivity/Kconfig \
	drivers/huawei_platform/connectivity/bcm/wifi/driver/bcmdhd/Kconfig \
	drivers/huawei_platform/connectivity/bcm/wifi/driver/Kconfig \
	drivers/huawei_platform/connectivity/bcm/wifi/Kconfig \
	drivers/huawei_platform/connectivity/bcm/gps/bcm47531/Kconfig \
	drivers/huawei_platform/connectivity/bcm/gps/bcm4752/Kconfig \
	drivers/huawei_platform/connectivity/bcm/gps/Kconfig \
	drivers/huawei_platform/connectivity/bcm/bluetooth/Kconfig \
	drivers/huawei_platform/connectivity/bcm/Kconfig \
	drivers/huawei_platform/connectivity/Kconfig \
	drivers/huawei_platform/camera_legacy/hi6220/Kconfig \
	drivers/huawei_platform/camera_legacy/Kconfig \
	drivers/huawei_platform/Kconfig \
	drivers/srecorder/Kconfig \
	drivers/sensors/Kconfig \
	drivers/android/Kconfig \
	drivers/gator/Kconfig \
	drivers/devdetect/Kconfig \
	drivers/hisi/hilog/Kconfig \
	drivers/hisi/perfhub/Kconfig \
	drivers/hisi/load_image/Kconfig \
	drivers/hisi/flp/Kconfig \
	drivers/hisi/hiperfdump/Kconfig \
	drivers/hisi/ivp/Kconfig \
	drivers/hisi/exmbist/Kconfig \
	drivers/hisi/memory_dump/Kconfig \
	drivers/hisi/hmpthresholds/Kconfig \
	drivers/hisi/misc/Kconfig \
	drivers/hisi/watchpoint/Kconfig \
	drivers/hisi/pm_common/Kconfig \
	drivers/hisi/slimbus/Kconfig \
	drivers/hisi/hi6402dsp/Kconfig \
	drivers/hisi/hi6402_vad/Kconfig \
	drivers/hisi/hi6402_mbhc/Kconfig \
	drivers/hisi/perf_stat/Kconfig \
	drivers/hisi/hi6421_spk/Kconfig \
	drivers/hisi/axi/Kconfig \
	drivers/hisi/hi3xxx/Kconfig \
	drivers/hisi/mcushell/Kconfig \
	drivers/hisi/tele_mntn/Kconfig \
	drivers/hisi/socp/Kconfig \
	drivers/hisi/noc/Kconfig \
	drivers/hisi/mutex_service/Kconfig \
	drivers/hisi/mailbox/hisi_mailbox/Kconfig \
	drivers/hisi/mailbox/Kconfig \
	drivers/hisi/hifi_mailbox/ipcm/Kconfig \
	drivers/hisi/hifi_mailbox/mailbox/Kconfig \
	drivers/hisi/hifi_mailbox/Kconfig \
	drivers/hisi/modem/drv/ocp/Kconfig \
	drivers/hisi/modem/drv/temperature/Kconfig \
	drivers/hisi/modem/drv/pm_om/Kconfig \
	drivers/hisi/modem/drv/om/Kconfig \
	drivers/hisi/modem/drv/psam/Kconfig \
	drivers/hisi/modem/drv/ipf/Kconfig \
	drivers/hisi/modem/drv/misc/Kconfig \
	drivers/hisi/modem/drv/cshell/Kconfig \
	drivers/hisi/modem/drv/sync/Kconfig \
	drivers/hisi/modem/drv/mem/Kconfig \
	drivers/hisi/modem/drv/sysctrl/Kconfig \
	drivers/hisi/modem/drv/udi/Kconfig \
	drivers/hisi/modem/drv/loadm/Kconfig \
	drivers/hisi/modem/drv/onoff/Kconfig \
	drivers/hisi/modem/drv/hwadp/Kconfig \
	drivers/hisi/modem/drv/s_memory/Kconfig \
	drivers/hisi/modem/drv/dsp/Kconfig \
	drivers/hisi/modem/drv/nvim/Kconfig \
	drivers/hisi/modem/drv/rfile/Kconfig \
	drivers/hisi/modem/drv/reset/Kconfig \
	drivers/hisi/modem/drv/balong_timer/Kconfig \
	drivers/hisi/modem/drv/icc/Kconfig \
	drivers/hisi/modem/drv/ipc/Kconfig \
	drivers/hisi/modem/drv/Kconfig \
	drivers/hisi/modem/Kconfig \
	drivers/hisi/mntn/blackbox/Kconfig \
	drivers/hisi/mntn/Kconfig \
	drivers/hisi/efuse/Kconfig \
	drivers/hisi/hi6xxx/Kconfig \
	drivers/hisi/soundtrigger/Kconfig \
	drivers/hisi/hifidsp/Kconfig \
	drivers/hisi/ddrc/Kconfig \
	drivers/hisi/adc/Kconfig \
	drivers/hisi/Kconfig \
	drivers/vcodec/jpu/v3_jpu/Kconfig \
	drivers/vcodec/jpu/v8_jpu/Kconfig \
	drivers/coresight/Kconfig \
	drivers/gpu/arm/r7p0-02rel0/platform/hisilicon/Kconfig \
	drivers/gpu/arm/r7p0-02rel0/platform/Kconfig \
	drivers/gpu/arm/r7p0-02rel0/Kconfig \
	drivers/gpu/arm/r8p0-01dev0/platform/hisilicon/Kconfig \
	drivers/gpu/arm/r8p0-01dev0/platform/Kconfig \
	drivers/gpu/arm/r8p0-01dev0/Kconfig \
	drivers/gpu/arm/Kconfig \
	drivers/reset/Kconfig \
	drivers/ipack/devices/Kconfig \
	drivers/ipack/carriers/Kconfig \
	drivers/ipack/Kconfig \
	drivers/irqchip/Kconfig \
	drivers/pwm/Kconfig \
	drivers/staging/vme/devices/Kconfig \
	drivers/vme/boards/Kconfig \
	drivers/vme/bridges/Kconfig \
	drivers/vme/Kconfig \
	drivers/ntb/Kconfig \
	drivers/iio/magnetometer/Kconfig \
	drivers/iio/light/Kconfig \
	drivers/iio/imu/inv_mpu6050/Kconfig \
	drivers/iio/imu/Kconfig \
	drivers/iio/gyro/Kconfig \
	drivers/iio/frequency/Kconfig \
	drivers/iio/dac/Kconfig \
	drivers/iio/common/st_sensors/Kconfig \
	drivers/iio/common/hid-sensors/Kconfig \
	drivers/iio/common/Kconfig \
	drivers/iio/amplifiers/Kconfig \
	drivers/iio/adc/Kconfig \
	drivers/iio/accel/Kconfig \
	drivers/iio/Kconfig \
	drivers/memory/Kconfig \
	drivers/extcon/Kconfig \
	drivers/devfreq/Kconfig \
	drivers/rpmsg/Kconfig \
	drivers/remoteproc/Kconfig \
	drivers/iommu/Kconfig \
	drivers/clocksource/Kconfig \
	drivers/hwspinlock/Kconfig \
	drivers/clk/mvebu/Kconfig \
	drivers/clk/hisilicon/Kconfig \
	drivers/clk/Kconfig \
	drivers/platform/goldfish/Kconfig \
	drivers/platform/x86/Kconfig \
	drivers/platform/Kconfig \
	drivers/staging/pasr/Kconfig \
	drivers/staging/dwc2/Kconfig \
	drivers/staging/netlogic/Kconfig \
	drivers/staging/goldfish/Kconfig \
	drivers/staging/zcache/Kconfig \
	drivers/staging/fwserial/Kconfig \
	drivers/staging/sb105x/Kconfig \
	drivers/staging/dgrp/Kconfig \
	drivers/staging/imx-drm/Kconfig \
	drivers/staging/ced1401/Kconfig \
	drivers/staging/silicom/Kconfig \
	drivers/staging/ti-soc-thermal/Kconfig \
	drivers/staging/csr/Kconfig \
	drivers/staging/gdm72xx/Kconfig \
	drivers/staging/ozwpan/Kconfig \
	drivers/staging/android/fiq_debugger/Kconfig \
	drivers/staging/android/ion/Kconfig \
	drivers/staging/android/Kconfig \
	drivers/staging/media/lirc/Kconfig \
	drivers/staging/media/solo6x10/Kconfig \
	drivers/staging/media/go7007/Kconfig \
	drivers/staging/media/dt3155v4l/Kconfig \
	drivers/staging/media/davinci_vpfe/Kconfig \
	drivers/staging/media/cxd2099/Kconfig \
	drivers/staging/media/as102/Kconfig \
	drivers/staging/media/Kconfig \
	drivers/staging/nvec/Kconfig \
	drivers/staging/ste_rmi4/Kconfig \
	drivers/staging/cptm1217/Kconfig \
	drivers/staging/speakup/Kconfig \
	drivers/staging/ft1000/Kconfig \
	drivers/staging/bcm/Kconfig \
	drivers/staging/keucr/Kconfig \
	drivers/staging/sbe-2t3e3/Kconfig \
	drivers/staging/quickstart/Kconfig \
	drivers/staging/tidspbridge/Kconfig \
	drivers/staging/xgifb/Kconfig \
	drivers/staging/cxt1e1/Kconfig \
	drivers/staging/crystalhd/Kconfig \
	drivers/staging/sm7xxfb/Kconfig \
	drivers/staging/wlags49_h25/Kconfig \
	drivers/staging/wlags49_h2/Kconfig \
	drivers/staging/iio/trigger/Kconfig \
	drivers/staging/iio/resolver/Kconfig \
	drivers/staging/iio/meter/Kconfig \
	drivers/staging/iio/magnetometer/Kconfig \
	drivers/staging/iio/light/Kconfig \
	drivers/staging/iio/impedance-analyzer/Kconfig \
	drivers/staging/iio/gyro/Kconfig \
	drivers/staging/iio/frequency/Kconfig \
	drivers/staging/iio/cdc/Kconfig \
	drivers/staging/iio/addac/Kconfig \
	drivers/staging/iio/adc/Kconfig \
	drivers/staging/iio/accel/Kconfig \
	drivers/staging/iio/Kconfig \
	drivers/staging/sep/Kconfig \
	drivers/staging/vt6656/Kconfig \
	drivers/staging/vt6655/Kconfig \
	drivers/staging/serqt_usb2/Kconfig \
	drivers/staging/octeon/Kconfig \
	drivers/staging/line6/Kconfig \
	drivers/staging/phison/Kconfig \
	drivers/staging/frontier/Kconfig \
	drivers/staging/rts5139/Kconfig \
	drivers/staging/rtl8712/Kconfig \
	drivers/staging/rtl8192e/rtl8192e/Kconfig \
	drivers/staging/rtl8192e/Kconfig \
	drivers/staging/rtl8192u/Kconfig \
	drivers/staging/rtl8187se/Kconfig \
	drivers/staging/panel/Kconfig \
	drivers/staging/asus_oled/Kconfig \
	drivers/staging/olpc_dcon/Kconfig \
	drivers/staging/comedi/Kconfig \
	drivers/staging/echo/Kconfig \
	drivers/staging/wlan-ng/Kconfig \
	drivers/staging/winbond/Kconfig \
	drivers/staging/usbip/Kconfig \
	drivers/staging/slicoss/Kconfig \
	drivers/staging/et131x/Kconfig \
	drivers/staging/Kconfig \
	drivers/xen/Kconfig \
	drivers/hv/Kconfig \
	drivers/virtio/Kconfig \
	drivers/virt/Kconfig \
	drivers/vlynq/Kconfig \
	drivers/vfio/pci/Kconfig \
	drivers/vfio/Kconfig \
	drivers/uio/Kconfig \
	drivers/auxdisplay/Kconfig \
	drivers/dca/Kconfig \
	drivers/dma/sh/Kconfig \
	drivers/dma/bestcomm/Kconfig \
	drivers/dma/Kconfig \
	drivers/rtc/Kconfig \
	drivers/edac/Kconfig \
	drivers/infiniband/ulp/isert/Kconfig \
	drivers/infiniband/ulp/iser/Kconfig \
	drivers/infiniband/ulp/srpt/Kconfig \
	drivers/infiniband/ulp/srp/Kconfig \
	drivers/infiniband/ulp/ipoib/Kconfig \
	drivers/infiniband/hw/ocrdma/Kconfig \
	drivers/infiniband/hw/nes/Kconfig \
	drivers/infiniband/hw/mlx4/Kconfig \
	drivers/infiniband/hw/cxgb4/Kconfig \
	drivers/infiniband/hw/cxgb3/Kconfig \
	drivers/infiniband/hw/amso1100/Kconfig \
	drivers/infiniband/hw/ehca/Kconfig \
	drivers/infiniband/hw/qib/Kconfig \
	drivers/infiniband/hw/ipath/Kconfig \
	drivers/infiniband/hw/mthca/Kconfig \
	drivers/infiniband/Kconfig \
	drivers/accessibility/Kconfig \
	drivers/switch/Kconfig \
	drivers/leds/trigger/Kconfig \
	drivers/leds/Kconfig \
	drivers/memstick/host/Kconfig \
	drivers/memstick/core/Kconfig \
	drivers/memstick/Kconfig \
	drivers/mmc/host/Kconfig \
	drivers/mmc/card/Kconfig \
	drivers/mmc/core/Kconfig \
	drivers/mmc/Kconfig \
	drivers/uwb/Kconfig \
	drivers/usb/gadget/Kconfig \
	drivers/usb/phy/Kconfig \
	drivers/usb/atm/Kconfig \
	drivers/usb/misc/sisusbvga/Kconfig \
	drivers/usb/misc/Kconfig \
	drivers/usb/serial/Kconfig \
	drivers/usb/chipidea/Kconfig \
	drivers/usb/dwc3/Kconfig \
	drivers/usb/susb/Kconfig \
	drivers/usb/image/Kconfig \
	drivers/usb/storage/Kconfig \
	drivers/usb/class/Kconfig \
	drivers/usb/renesas_usbhs/Kconfig \
	drivers/usb/musb/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/wusbcore/Kconfig \
	drivers/usb/mon/Kconfig \
	drivers/usb/core/Kconfig \
	drivers/usb/Kconfig \
	drivers/hid/i2c-hid/Kconfig \
	drivers/hid/usbhid/Kconfig \
	drivers/hid/Kconfig \
	sound/oss/Kconfig \
	sound/soc/generic/Kconfig \
	sound/soc/codecs/Kconfig \
	sound/soc/ux500/Kconfig \
	sound/soc/txx9/Kconfig \
	sound/soc/tegra/Kconfig \
	sound/soc/sh/Kconfig \
	sound/soc/s6000/Kconfig \
	sound/soc/samsung/Kconfig \
	sound/soc/pxa/Kconfig \
	sound/soc/mxs/Kconfig \
	sound/soc/mid-x86/Kconfig \
	sound/soc/kirkwood/Kconfig \
	sound/soc/omap/Kconfig \
	sound/soc/nuc900/Kconfig \
	sound/soc/jz4740/Kconfig \
	sound/soc/hisilicon/Kconfig \
	sound/soc/fsl/Kconfig \
	sound/soc/dwc/Kconfig \
	sound/soc/davinci/Kconfig \
	sound/soc/cirrus/Kconfig \
	sound/soc/blackfin/Kconfig \
	sound/soc/au1x/Kconfig \
	sound/soc/atmel/Kconfig \
	sound/soc/Kconfig \
	sound/parisc/Kconfig \
	sound/sparc/Kconfig \
	sound/pcmcia/Kconfig \
	sound/firewire/Kconfig \
	sound/usb/Kconfig \
	sound/sh/Kconfig \
	sound/mips/Kconfig \
	sound/spi/Kconfig \
	sound/atmel/Kconfig \
	sound/arm/Kconfig \
	sound/aoa/soundbus/Kconfig \
	sound/aoa/codecs/Kconfig \
	sound/aoa/fabrics/Kconfig \
	sound/aoa/Kconfig \
	sound/ppc/Kconfig \
	sound/pci/hda/Kconfig \
	sound/pci/Kconfig \
	sound/isa/Kconfig \
	sound/drivers/Kconfig \
	sound/core/seq/Kconfig \
	sound/core/Kconfig \
	sound/oss/dmasound/Kconfig \
	sound/Kconfig \
	drivers/video/logo/Kconfig \
	drivers/video/console/Kconfig \
	drivers/video/adf/Kconfig \
	drivers/video/backlight/Kconfig \
	drivers/video/mmp/fb/Kconfig \
	drivers/video/mmp/panel/Kconfig \
	drivers/video/mmp/hw/Kconfig \
	drivers/video/mmp/Kconfig \
	drivers/video/hisilicon/Kconfig \
	drivers/video/exynos/Kconfig \
	drivers/video/omap2/displays/Kconfig \
	drivers/video/omap2/omapfb/Kconfig \
	drivers/video/omap2/dss/Kconfig \
	drivers/video/omap2/Kconfig \
	drivers/video/omap/Kconfig \
	drivers/video/geode/Kconfig \
	drivers/gpu/stub/Kconfig \
	drivers/gpu/drm/radeon/Kconfig \
	drivers/gpu/drm/Kconfig \
	drivers/gpu/vga/Kconfig \
	drivers/char/agp/Kconfig \
	drivers/video/Kconfig \
	drivers/vcodec/vpp/Kconfig \
	drivers/videocom/videoconfig/Kconfig \
	drivers/videocom/videoharden/Kconfig \
	drivers/videocom/mntn/Kconfig \
	drivers/videocom/Kconfig \
	drivers/vcodec/chipmedia/vpu/Kconfig \
	drivers/vcodec/chipmedia/Kconfig \
	drivers/vcodec/imagination/VXD395/encode/Kconfig \
	drivers/vcodec/imagination/VXD395/vdec/Kconfig \
	drivers/vcodec/imagination/VXD395/imgvideo/Kconfig \
	drivers/vcodec/imagination/VXD395/Kconfig \
	drivers/vcodec/imagination/D5500/encode/Kconfig \
	drivers/vcodec/imagination/D5500/vdec/Kconfig \
	drivers/vcodec/imagination/D5500/imgvideo/Kconfig \
	drivers/vcodec/imagination/D5500/Kconfig \
	drivers/media/dvb-frontends/Kconfig \
	drivers/media/tuners/Kconfig \
	drivers/media/i2c/soc_camera/Kconfig \
	drivers/media/i2c/smiapp/Kconfig \
	drivers/media/i2c/m5mols/Kconfig \
	drivers/media/i2c/cx25840/Kconfig \
	drivers/media/i2c/Kconfig \
	drivers/media/common/siano/Kconfig \
	drivers/media/common/saa7146/Kconfig \
	drivers/media/common/b2c2/Kconfig \
	drivers/media/common/Kconfig \
	drivers/media/firewire/Kconfig \
	drivers/media/huawei/camera/extisp/Kconfig \
	drivers/media/huawei/camera/platform/hi6250/Kconfig \
	drivers/media/huawei/camera/platform/hi3635/Kconfig \
	drivers/media/huawei/camera/platform/hi3650/Kconfig \
	drivers/media/huawei/camera/sensor/ois/Kconfig \
	drivers/media/huawei/camera/sensor/flash/Kconfig \
	drivers/media/huawei/camera/sensor/vcm/Kconfig \
	drivers/media/huawei/camera/sensor/Kconfig \
	drivers/media/huawei/camera/hisp/hisp110/Kconfig \
	drivers/media/huawei/camera/hisp/hisp100/Kconfig \
	drivers/media/huawei/camera/hisp/Kconfig \
	drivers/media/huawei/camera/Kconfig \
	drivers/media/huawei/Kconfig \
	drivers/media/radio/wl128x/Kconfig \
	drivers/media/radio/si470x/Kconfig \
	drivers/media/radio/Kconfig \
	drivers/media/parport/Kconfig \
	drivers/media/mmc/siano/Kconfig \
	drivers/media/mmc/Kconfig \
	drivers/media/platform/ovisp22/fake_capture/Kconfig \
	drivers/media/platform/ovisp22/capture/Kconfig \
	drivers/media/platform/ovisp23/flash/Kconfig \
	drivers/media/platform/ovisp23/isp/Kconfig \
	drivers/media/platform/ovisp23/vcm/Kconfig \
	drivers/media/platform/ovisp23/sensor/Kconfig \
	drivers/media/platform/ovisp23/Kconfig \
	drivers/media/platform/s5p-tv/Kconfig \
	drivers/media/platform/exynos4-is/Kconfig \
	drivers/media/platform/soc_camera/Kconfig \
	drivers/media/platform/blackfin/Kconfig \
	drivers/media/platform/omap/Kconfig \
	drivers/media/platform/davinci/Kconfig \
	drivers/media/platform/marvell-ccic/Kconfig \
	drivers/media/platform/Kconfig \
	drivers/media/pci/ddbridge/Kconfig \
	drivers/media/pci/ngene/Kconfig \
	drivers/media/pci/mantis/Kconfig \
	drivers/media/pci/pt1/Kconfig \
	drivers/media/pci/dm1105/Kconfig \
	drivers/media/pci/pluto2/Kconfig \
	drivers/media/pci/b2c2/Kconfig \
	drivers/media/pci/ttpci/Kconfig \
	drivers/media/pci/saa7164/Kconfig \
	drivers/media/pci/saa7134/Kconfig \
	drivers/media/pci/bt8xx/Kconfig \
	drivers/media/pci/cx88/Kconfig \
	drivers/media/pci/cx25821/Kconfig \
	drivers/media/pci/cx23885/Kconfig \
	drivers/media/pci/cx18/Kconfig \
	drivers/media/pci/saa7146/Kconfig \
	drivers/media/pci/zoran/Kconfig \
	drivers/media/pci/ivtv/Kconfig \
	drivers/media/pci/sta2x11/Kconfig \
	drivers/media/pci/meye/Kconfig \
	drivers/media/pci/Kconfig \
	drivers/media/usb/em28xx/Kconfig \
	drivers/media/usb/b2c2/Kconfig \
	drivers/media/usb/siano/Kconfig \
	drivers/media/usb/ttusb-dec/Kconfig \
	drivers/media/usb/ttusb-budget/Kconfig \
	drivers/media/usb/dvb-usb-v2/Kconfig \
	drivers/media/usb/dvb-usb/Kconfig \
	drivers/media/usb/tm6000/Kconfig \
	drivers/media/usb/cx231xx/Kconfig \
	drivers/media/usb/au0828/Kconfig \
	drivers/media/usb/stk1160/Kconfig \
	drivers/media/usb/usbvision/Kconfig \
	drivers/media/usb/tlg2300/Kconfig \
	drivers/media/usb/hdpvr/Kconfig \
	drivers/media/usb/pvrusb2/Kconfig \
	drivers/media/usb/sn9c102/Kconfig \
	drivers/media/usb/s2255/Kconfig \
	drivers/media/usb/stkwebcam/Kconfig \
	drivers/media/usb/zr364xx/Kconfig \
	drivers/media/usb/cpia2/Kconfig \
	drivers/media/usb/pwc/Kconfig \
	drivers/media/usb/gspca/gl860/Kconfig \
	drivers/media/usb/gspca/stv06xx/Kconfig \
	drivers/media/usb/gspca/m5602/Kconfig \
	drivers/media/usb/gspca/Kconfig \
	drivers/media/usb/uvc/Kconfig \
	drivers/media/usb/Kconfig \
	drivers/media/rc/keymaps/Kconfig \
	drivers/media/rc/Kconfig \
	drivers/media/dvb-core/Kconfig \
	drivers/media/v4l2-core/Kconfig \
	drivers/media/Kconfig \
	drivers/regulator/Kconfig \
	drivers/mfd/Kconfig \
	drivers/bcma/Kconfig \
	drivers/ssb/Kconfig \
	drivers/watchdog/Kconfig \
	drivers/thermal/Kconfig \
	drivers/hwmon/pmbus/Kconfig \
	drivers/hwmon/Kconfig \
	drivers/power/avs/Kconfig \
	drivers/power/reset/Kconfig \
	drivers/power/hisi/coul/Kconfig \
	drivers/power/hisi/charger/Kconfig \
	drivers/power/hisi/Kconfig \
	drivers/power/huawei/Kconfig \
	drivers/power/Kconfig \
	drivers/w1/slaves/Kconfig \
	drivers/w1/masters/Kconfig \
	drivers/w1/Kconfig \
	drivers/gpio/Kconfig \
	drivers/pinctrl/vt8500/Kconfig \
	drivers/pinctrl/spear/Kconfig \
	drivers/pinctrl/sh-pfc/Kconfig \
	drivers/pinctrl/mvebu/Kconfig \
	drivers/pinctrl/Kconfig \
	drivers/ptp/Kconfig \
	drivers/pps/generators/Kconfig \
	drivers/pps/clients/Kconfig \
	drivers/pps/Kconfig \
	drivers/hsi/clients/Kconfig \
	drivers/hsi/Kconfig \
	drivers/ssbi/Kconfig \
	drivers/spi/Kconfig \
	drivers/i2c/busses/Kconfig \
	drivers/i2c/algos/Kconfig \
	drivers/i2c/muxes/Kconfig \
	drivers/i2c/Kconfig \
	drivers/s390/char/Kconfig \
	drivers/char/tpm/Kconfig \
	drivers/char/pcmcia/Kconfig \
	drivers/char/hw_random/Kconfig \
	drivers/char/ipmi/Kconfig \
	drivers/tty/hvc/Kconfig \
	drivers/tty/serial/8250/Kconfig \
	drivers/tty/serial/Kconfig \
	drivers/tty/Kconfig \
	drivers/char/Kconfig \
	drivers/input/gameport/Kconfig \
	drivers/input/serio/Kconfig \
	drivers/input/misc/Kconfig \
	drivers/input/touchscreen/shtsc/Kconfig \
	drivers/input/touchscreen/synaptics_dsx/Kconfig \
	drivers/input/touchscreen/cypress-dt/Kconfig \
	drivers/input/touchscreen/Kconfig \
	drivers/input/tablet/Kconfig \
	drivers/input/joystick/iforce/Kconfig \
	drivers/input/joystick/Kconfig \
	drivers/input/mouse/Kconfig \
	drivers/input/keyboard/Kconfig \
	drivers/input/Kconfig \
	drivers/isdn/hardware/mISDN/Kconfig \
	drivers/isdn/mISDN/Kconfig \
	drivers/isdn/hysdn/Kconfig \
	drivers/isdn/gigaset/Kconfig \
	drivers/isdn/hardware/eicon/Kconfig \
	drivers/isdn/hardware/avm/Kconfig \
	drivers/isdn/hardware/Kconfig \
	drivers/isdn/capi/Kconfig \
	drivers/isdn/act2000/Kconfig \
	drivers/isdn/sc/Kconfig \
	drivers/isdn/pcbit/Kconfig \
	drivers/isdn/icn/Kconfig \
	drivers/isdn/hisax/Kconfig \
	drivers/isdn/i4l/Kconfig \
	drivers/isdn/Kconfig \
	drivers/net/hyperv/Kconfig \
	drivers/net/ieee802154/Kconfig \
	drivers/net/wan/Kconfig \
	drivers/net/wimax/i2400m/Kconfig \
	drivers/net/wimax/Kconfig \
	drivers/net/wireless/mwifiex/Kconfig \
	drivers/net/wireless/zd1211rw/Kconfig \
	drivers/net/wireless/ti/wlcore/Kconfig \
	drivers/net/wireless/ti/wl18xx/Kconfig \
	drivers/net/wireless/ti/wl12xx/Kconfig \
	drivers/net/wireless/ti/wl1251/Kconfig \
	drivers/net/wireless/ti/Kconfig \
	drivers/net/wireless/rtlwifi/Kconfig \
	drivers/net/wireless/rt2x00/Kconfig \
	drivers/net/wireless/p54/Kconfig \
	drivers/net/wireless/orinoco/Kconfig \
	drivers/net/wireless/libertas/Kconfig \
	drivers/net/wireless/iwlegacy/Kconfig \
	drivers/net/wireless/iwlwifi/Kconfig \
	drivers/net/wireless/ipw2x00/Kconfig \
	drivers/net/wireless/hostap/Kconfig \
	drivers/net/wireless/brcm80211/Kconfig \
	drivers/net/wireless/b43legacy/Kconfig \
	drivers/net/wireless/b43/Kconfig \
	drivers/net/wireless/ath/wil6210/Kconfig \
	drivers/net/wireless/ath/ar5523/Kconfig \
	drivers/net/wireless/ath/ath6kl/Kconfig \
	drivers/net/wireless/ath/carl9170/Kconfig \
	drivers/net/wireless/ath/ath9k/Kconfig \
	drivers/net/wireless/ath/ath5k/Kconfig \
	drivers/net/wireless/ath/Kconfig \
	drivers/net/wireless/rtl818x/Kconfig \
	drivers/net/wireless/Kconfig \
	drivers/net/usb/Kconfig \
	drivers/s390/net/Kconfig \
	drivers/net/slip/Kconfig \
	drivers/net/ppp/Kconfig \
	drivers/net/plip/Kconfig \
	drivers/net/phy/Kconfig \
	drivers/net/hippi/Kconfig \
	drivers/net/fddi/Kconfig \
	drivers/net/ethernet/xircom/Kconfig \
	drivers/net/ethernet/xilinx/Kconfig \
	drivers/net/ethernet/wiznet/Kconfig \
	drivers/net/ethernet/via/Kconfig \
	drivers/net/ethernet/tundra/Kconfig \
	drivers/net/ethernet/toshiba/Kconfig \
	drivers/net/ethernet/tile/Kconfig \
	drivers/net/ethernet/ti/Kconfig \
	drivers/net/ethernet/tehuti/Kconfig \
	drivers/net/ethernet/sun/Kconfig \
	drivers/net/ethernet/stmicro/stmmac/Kconfig \
	drivers/net/ethernet/stmicro/Kconfig \
	drivers/net/ethernet/smsc/Kconfig \
	drivers/net/ethernet/sgi/Kconfig \
	drivers/net/ethernet/sfc/Kconfig \
	drivers/net/ethernet/sis/Kconfig \
	drivers/net/ethernet/silan/Kconfig \
	drivers/net/ethernet/seeq/Kconfig \
	drivers/net/ethernet/rdc/Kconfig \
	drivers/net/ethernet/renesas/Kconfig \
	drivers/net/ethernet/realtek/Kconfig \
	drivers/net/ethernet/qlogic/Kconfig \
	drivers/net/ethernet/pasemi/Kconfig \
	drivers/net/ethernet/packetengines/Kconfig \
	drivers/net/ethernet/oki-semi/pch_gbe/Kconfig \
	drivers/net/ethernet/oki-semi/Kconfig \
	drivers/net/ethernet/octeon/Kconfig \
	drivers/net/ethernet/nxp/Kconfig \
	drivers/net/ethernet/nvidia/Kconfig \
	drivers/net/ethernet/nuvoton/Kconfig \
	drivers/net/ethernet/8390/Kconfig \
	drivers/net/ethernet/natsemi/Kconfig \
	drivers/net/ethernet/myricom/Kconfig \
	drivers/net/ethernet/microchip/Kconfig \
	drivers/net/ethernet/micrel/Kconfig \
	drivers/net/ethernet/mellanox/mlx4/Kconfig \
	drivers/net/ethernet/mellanox/Kconfig \
	drivers/net/ethernet/marvell/Kconfig \
	drivers/net/ethernet/icplus/Kconfig \
	drivers/net/ethernet/xscale/Kconfig \
	drivers/net/ethernet/i825xx/Kconfig \
	drivers/net/ethernet/intel/Kconfig \
	drivers/net/ethernet/ibm/emac/Kconfig \
	drivers/net/ethernet/ibm/Kconfig \
	drivers/net/ethernet/hp/Kconfig \
	drivers/net/ethernet/fujitsu/Kconfig \
	drivers/net/ethernet/freescale/fs_enet/Kconfig \
	drivers/net/ethernet/freescale/Kconfig \
	drivers/net/ethernet/faraday/Kconfig \
	drivers/net/ethernet/neterion/Kconfig \
	drivers/net/ethernet/emulex/benet/Kconfig \
	drivers/net/ethernet/emulex/Kconfig \
	drivers/net/ethernet/dlink/Kconfig \
	drivers/net/ethernet/dec/tulip/Kconfig \
	drivers/net/ethernet/dec/Kconfig \
	drivers/net/ethernet/davicom/Kconfig \
	drivers/net/ethernet/cisco/enic/Kconfig \
	drivers/net/ethernet/cisco/Kconfig \
	drivers/net/ethernet/cirrus/Kconfig \
	drivers/net/ethernet/chelsio/Kconfig \
	drivers/net/ethernet/calxeda/Kconfig \
	drivers/net/ethernet/brocade/bna/Kconfig \
	drivers/net/ethernet/brocade/Kconfig \
	drivers/net/ethernet/broadcom/Kconfig \
	drivers/net/ethernet/adi/Kconfig \
	drivers/net/ethernet/cadence/Kconfig \
	drivers/net/ethernet/atheros/Kconfig \
	drivers/net/ethernet/apple/Kconfig \
	drivers/net/ethernet/amd/Kconfig \
	drivers/net/ethernet/alteon/Kconfig \
	drivers/net/ethernet/aeroflex/Kconfig \
	drivers/net/ethernet/adaptec/Kconfig \
	drivers/net/ethernet/3com/Kconfig \
	drivers/net/ethernet/Kconfig \
	drivers/net/dsa/Kconfig \
	drivers/vhost/Kconfig \
	drivers/net/caif/Kconfig \
	drivers/atm/Kconfig \
	drivers/net/arcnet/Kconfig \
	drivers/net/team/Kconfig \
	drivers/net/Kconfig \
	drivers/macintosh/Kconfig \
	drivers/message/i2o/Kconfig \
	drivers/firewire/Kconfig \
	drivers/message/fusion/Kconfig \
	drivers/target/sbp/Kconfig \
	drivers/target/iscsi/Kconfig \
	drivers/target/tcm_fc/Kconfig \
	drivers/target/loopback/Kconfig \
	drivers/target/Kconfig \
	drivers/md/persistent-data/Kconfig \
	drivers/md/bcache/Kconfig \
	drivers/md/Kconfig \
	drivers/ata/Kconfig \
	drivers/scsi/osd/Kconfig \
	drivers/scsi/device_handler/Kconfig \
	drivers/scsi/pcmcia/Kconfig \
	drivers/scsi/csiostor/Kconfig \
	drivers/scsi/arm/Kconfig \
	drivers/scsi/qla4xxx/Kconfig \
	drivers/scsi/qla2xxx/Kconfig \
	drivers/scsi/ufs/Kconfig \
	drivers/scsi/mpt3sas/Kconfig \
	drivers/scsi/mpt2sas/Kconfig \
	drivers/scsi/megaraid/Kconfig.megaraid \
	drivers/scsi/mvsas/Kconfig \
	drivers/scsi/aic94xx/Kconfig \
	drivers/scsi/aic7xxx/Kconfig.aic79xx \
	drivers/scsi/aic7xxx/Kconfig.aic7xxx \
	drivers/scsi/be2iscsi/Kconfig \
	drivers/scsi/bnx2fc/Kconfig \
	drivers/scsi/bnx2i/Kconfig \
	drivers/scsi/cxgbi/cxgb4i/Kconfig \
	drivers/scsi/cxgbi/cxgb3i/Kconfig \
	drivers/scsi/cxgbi/Kconfig \
	drivers/scsi/libsas/Kconfig \
	drivers/scsi/Kconfig \
	drivers/ide/Kconfig \
	drivers/misc_via/Kconfig \
	drivers/misc/drv2605/Kconfig \
	drivers/misc/vmw_vmci/Kconfig \
	drivers/misc/hilog/Kconfig \
	drivers/misc/mei/Kconfig \
	drivers/misc/altera-stapl/Kconfig \
	drivers/misc/carma/Kconfig \
	drivers/misc/lis3lv02d/Kconfig \
	drivers/misc/ti-st/Kconfig \
	drivers/misc/cb710/Kconfig \
	drivers/misc/eeprom/Kconfig \
	drivers/misc/c2port/Kconfig \
	drivers/misc/Kconfig \
	drivers/s390/block/Kconfig \
	drivers/block/drbd/Kconfig \
	drivers/block/zram/Kconfig \
	drivers/block/mtip32xx/Kconfig \
	drivers/block/paride/Kconfig \
	drivers/block/Kconfig \
	drivers/pnp/pnpacpi/Kconfig \
	drivers/pnp/pnpbios/Kconfig \
	drivers/pnp/isapnp/Kconfig \
	drivers/pnp/Kconfig \
	drivers/parport/Kconfig \
	drivers/of/Kconfig \
	drivers/mtd/ubi/Kconfig \
	drivers/mtd/lpddr/Kconfig \
	drivers/mtd/onenand/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/devices/Kconfig \
	drivers/mtd/maps/Kconfig \
	drivers/mtd/chips/Kconfig \
	drivers/mtd/Kconfig \
	drivers/connector/Kconfig \
	drivers/bus/Kconfig \
	drivers/base/regmap/Kconfig \
	drivers/base/Kconfig \
	drivers/atfdriver/Kconfig \
	drivers/tzdriver/Kconfig \
	drivers/Kconfig \
	drivers/nfc/microread/Kconfig \
	drivers/nfc/pn544/Kconfig \
	drivers/nfc/Kconfig \
	net/nfc/hci/Kconfig \
	net/nfc/nci/Kconfig \
	net/nfc/Kconfig \
	net/ceph/Kconfig \
	net/caif/Kconfig \
	net/9p/Kconfig \
	net/rfkill/Kconfig \
	net/wimax/Kconfig \
	net/mac80211/Kconfig \
	net/wireless/Kconfig \
	net/rxrpc/Kconfig \
	drivers/bluetooth/Kconfig \
	net/bluetooth/hidp/Kconfig \
	net/bluetooth/cmtp/Kconfig \
	net/bluetooth/bnep/Kconfig \
	net/bluetooth/rfcomm/Kconfig \
	net/bluetooth/Kconfig \
	drivers/net/irda/Kconfig \
	net/irda/ircomm/Kconfig \
	net/irda/irnet/Kconfig \
	net/irda/irlan/Kconfig \
	net/irda/Kconfig \
	drivers/net/can/softing/Kconfig \
	drivers/net/can/usb/Kconfig \
	drivers/net/can/cc770/Kconfig \
	drivers/net/can/c_can/Kconfig \
	drivers/net/can/sja1000/Kconfig \
	drivers/net/can/mscan/Kconfig \
	drivers/net/can/Kconfig \
	net/can/Kconfig \
	drivers/net/hamradio/Kconfig \
	net/ax25/Kconfig \
	net/netlink/Kconfig \
	net/vmw_vsock/Kconfig \
	net/openvswitch/Kconfig \
	net/batman-adv/Kconfig \
	net/dns_resolver/Kconfig \
	net/dcb/Kconfig \
	net/sched/Kconfig \
	net/mac802154/Kconfig \
	net/ieee802154/Kconfig \
	net/phonet/Kconfig \
	net/lapb/Kconfig \
	net/x25/Kconfig \
	drivers/net/appletalk/Kconfig \
	net/ipx/Kconfig \
	net/llc/Kconfig \
	net/decnet/Kconfig \
	net/8021q/Kconfig \
	net/dsa/Kconfig \
	net/bridge/Kconfig \
	net/802/Kconfig \
	net/l2tp/Kconfig \
	net/atm/Kconfig \
	net/tipc/Kconfig \
	net/rds/Kconfig \
	net/sctp/Kconfig \
	net/dccp/ccids/Kconfig \
	net/dccp/Kconfig \
	net/bridge/netfilter/Kconfig \
	net/decnet/netfilter/Kconfig \
	net/ipv6/netfilter/Kconfig \
	net/ipv4/netfilter/Kconfig \
	net/netfilter/ipvs/Kconfig \
	net/netfilter/ipset/Kconfig \
	net/netfilter/Kconfig \
	net/netlabel/Kconfig \
	net/ipv6/Kconfig \
	net/ipv4/Kconfig \
	net/iucv/Kconfig \
	net/xfrm/Kconfig \
	net/unix/Kconfig \
	net/packet/Kconfig \
	net/Kconfig \
	fs/Kconfig.binfmt \
	drivers/rapidio/switches/Kconfig \
	drivers/rapidio/devices/Kconfig \
	drivers/rapidio/Kconfig \
	drivers/pci/hotplug/Kconfig \
	drivers/pcmcia/Kconfig \
	drivers/eisa/Kconfig \
	drivers/pci/Kconfig \
	drivers/pci/pcie/aer/Kconfig.debug \
	drivers/pci/pcie/aer/Kconfig \
	drivers/pci/pcie/Kconfig \
	drivers/idle/Kconfig \
	drivers/cpuidle/Kconfig.arm64 \
	drivers/cpuidle/Kconfig \
	drivers/cpufreq/Kconfig.powerpc \
	drivers/cpufreq/Kconfig.arm \
	drivers/cpufreq/Kconfig.x86 \
	drivers/cpufreq/Kconfig \
	drivers/sfi/Kconfig \
	drivers/acpi/apei/Kconfig \
	drivers/acpi/Kconfig \
	kernel/power/Kconfig \
	kernel/Kconfig.hz \
	mm/Kconfig \
	kernel/Kconfig.preempt \
	arch/x86/Kconfig.cpu \
	arch/x86/lguest/Kconfig \
	arch/x86/xen/Kconfig \
	kernel/Kconfig.freezer \
	kernel/Kconfig.locks \
	block/Kconfig.iosched \
	block/partitions/Kconfig \
	block/Kconfig \
	kernel/gcov/Kconfig \
	arch/Kconfig \
	usr/Kconfig \
	kernel/time/Kconfig \
	kernel/irq/Kconfig \
	init/Kconfig \
	arch/x86/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(KERNELVERSION)" "3.10.86"
include/config/auto.conf: FORCE
endif
ifneq "$(ARCH)" "x86"
include/config/auto.conf: FORCE
endif
ifneq "$(SRCARCH)" "x86"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
