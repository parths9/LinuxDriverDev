obj-m += first_driver.o
 
KDIR = /home/pi/linux-c0d4ab94e37991db311b0d4e955a349e359fc73a
 
all:
	make -C $(KDIR)  M=/home/pi/LinuxDriverDev/first_driver  modules

clean:
	make -C $(KDIR)  M=/home/pi/LinuxDriverDev/first_driver  clean
