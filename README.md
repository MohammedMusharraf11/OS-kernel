
# OS-Kernel

This repository contains simple Linux Kernel modules for educational purposes.

## Author

[**Mohammed Musharraf**](https://github.com/MohammedMusharraf11)

## Repository

GitHub: [OS-Kernel](https://github.com/MohammedMusharraf11/OS-Kernel)

## Structure

- `kernel-module.c`: A simple Linux Kernel module (LKM) implementation.
- `user-module.c`: A user space program that interacts with the kernel module.
- `Makefile`: Used to build and manage the kernel module.

## Requirements

Make sure you have the following installed on your Linux system:

```bash
sudo apt-get update
sudo apt-get install -y build-essential linux-headers-$(uname -r)
```

## Building the Kernel Module

```bash
make
```

## Loading the Kernel Module

```bash
sudo insmod kernel-module.ko
```

## Checking Kernel Logs

```bash
dmesg | tail -10
```

## Running the User Program

```bash
gcc user-module.c -o user-module
./user-module
```

## Unloading the Kernel Module

```bash
sudo rmmod kernel-module
dmesg | tail -10
```

## Cleaning Up

```bash
make clean
```

---

> **Note:** Ensure you are running this on a native Linux machine or in a VM with kernel headers properly installed. It may not work as expected inside a Docker container without privileged access.
