<<<<<<< HEAD
#
# Makefile for kernel slimbus framework.
#
obj-$(CONFIG_SLIMBUS)			+= slimbus.o
obj-$(CONFIG_SLIMBUS_MSM_CTRL)		+= slim-msm.o slim-msm-ctrl.o
obj-$(CONFIG_SLIMBUS_MSM_NGD)		+= slim-msm.o slim-msm-ngd.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for kernel SLIMbus framework.
#
obj-$(CONFIG_SLIMBUS)			+= slimbus.o
slimbus-y				:= core.o messaging.o sched.o stream.o

#Controllers
obj-$(CONFIG_SLIM_QCOM_CTRL)		+= slim-qcom-ctrl.o
slim-qcom-ctrl-y			:= qcom-ctrl.o

obj-$(CONFIG_SLIM_QCOM_NGD_CTRL)	+= slim-qcom-ngd-ctrl.o
slim-qcom-ngd-ctrl-y			:= qcom-ngd-ctrl.o
>>>>>>> v4.19.83
