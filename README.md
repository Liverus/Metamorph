<br/>
<p align="center">
  <a href="https://github.com/Liverus/Metamorph" src="Metamorph.png">
    <img src="" alt="Logo" width="80" height="80">
  </a>
  <br>
  <br>
  <br>
</p>

## About The Project

Metamorph is a Windows HWID-Spoofer project for the game SCP:SL. It internally uses kd-mapper to load an unsigned driver for the kernel part.
The spoofer has been made specifically for changing what SCP:SL collects and not anything else. Therefore, Metamorph will hardly have any value outside of its original purpose.

The code is garbage and pasted but it serves its purpose. 



Tested on the 12/02/2024.

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

The code relies on Mutante by SamuelTulac which is as he stated, "heavily outdated". Therefore, consider this project as the same.
There are still a few remaining detection vectors:
* IP Address
* User Security ID (SID)
* Network Adapters (Some may fail to change)

# Credits

* [Mutante](https://github.com/SamuelTulach/mutante) by SamuelTulac
* [HWID](https://github.com/btbd/hwid) by BTBD
* [KDMapper](https://github.com/TheCruZ/kdmapper) by TheCruZ
