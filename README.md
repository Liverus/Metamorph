<br/>
<p align="center">
  <a href="https://github.com/Liverus/Metamorph">
    <img src="Metamorph.png" alt="Metamorph" width="240" height="240">
  </a>
  <br>
  <br>
  <br>
</p>

> [!IMPORTANT]
> This project was never intended to be maintained.
> 
> Please stop asking me to update this project.
> Take a look at Midnight if you want something better.

# About

Metamorph is a Windows HWID-Spoofer project for the game SCP:SL. It internally uses kd-mapper to load an unsigned driver for the kernel part.
The spoofer has been made specifically for changing what SCP:SL collects and not anything else. Therefore, Metamorph will hardly have any value outside of its original purpose.

The code is garbage and pasted but it serves its purpose. I take no credit for the creation of this abomination except for the spaghetti code, it's all me.



Tested on the 12/02/2024 for Windows 11 (10.0.22631).

# Features

## Windows
* ComputerName
* Digital Product ID
* Product ID
* Install Date
* Machine GUID
* Wallpaper (To default dark)

## Misc
* ARP Table
* Network Adapters MAC
* SMBIOS
* Motherboard MAC
* Disks

# Limitations

The code relies on Mutante by SamuelTulach which is as he stated, "heavily outdated". Therefore, consider this project as the same.
There are still a few remaining detection vectors:
* IP Address
* User Security ID (SID)
* Network Adapters (Some may fail to change)

# Credits

* [Mutante](https://github.com/SamuelTulach/mutante) by SamuelTulac
* [HWID](https://github.com/btbd/hwid) by BTBD
* [KDMapper](https://github.com/TheCruZ/kdmapper) by TheCruZ
