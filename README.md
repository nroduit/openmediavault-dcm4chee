# OpenMediaVault plugin for dcm4chee and Weasis #

## PACS plugin in OpenMediaVault web interface ##
![OpenMediaVault PACS plugin](openmediavault1.png)

## PACS logs ##
![OpenMediaVault PACS logger](openmediavault2.png)

# Install dcm4chee on OpenMediaVault #

## Prerequisites ##
1. Download iso at https://sourceforge.net/projects/openmediavault/files/2.1/
1. Install it and configure it according to your locale and network setting

References for installation:

* http://www.openmediavault.org/
* http://wiki.openmediavault.org/
* http://en.jose-crispim.pt/artigos/armazenamento/armaz_art/06_openmediavault.html (old version)

## Configure openmediavault ##
Once you have installed the system, these are the default access credentials.

* Client (SSH, console)   
  User: root   
  Password: (set during installation)

* WebGUI   
  User: admin   
  Password: openmediavault   

In case of the WebGUI access issue, type the command `omv-firstaid` in a console.

### Activate SSH ###
In Services > SSH, enable, save and apply. 

### Upgrade packages ###
In System > Update Manager, select all the packages and click on "Upgrade".

## Install/Upgrade dcm4chee and Weasis ##
1. Download the latest plugin (openmediavault-dcm4chee_x.x.x_all.deb) at https://github.com/nroduit/openmediavault-dcm4chee/releases
1. In System > Plugin, click on "Upload" and select the file "openmediavault-dcm4chee_x.x.x_all.deb"
1. In Services > PACS, click on "Download and install/upgrade dcm4chee"

# Install dcm4chee on Debian-based distribution #

## Install/Upgrade dcm4chee and Weasis ##

1. Download the latest dcm4chee-mysql_x.x.x_all.deb and dcm4chee-weasis_x.x.x_all.deb at https://github.com/nroduit/openmediavault-dcm4chee/releases
1. `sudo dpkg -i dcm4chee-mysql_x.x.x_all.deb`
1. `sudo apt-get -f install`
1. `sudo dpkg -i dcm4chee-weasis_x.x.x_all.deb`

## Remove dcm4chee and Weasis ##

* `sudo apt-get remove dcm4chee-weasis` or `sudo dpkg -r dcm4chee-weasis`
* `sudo apt-get remove dcm4chee-mysql` or `sudo dpkg -r dcm4chee-mysql`      
  => will remove the binaries, but not the configuration or data files created by dcm4chee-mysql. It will also leave dependencies installed with it on installation time untouched.
  
  
Note: for removing all the files (including the generated files), type `sudo apt-get purge dcm4chee-mysql` or `sudo dpkg -P dcm4chee-mysql`. This will remove about everything regarding the package dcm4chee-mysql (all the content of /var/lib/dcm4chee/ and drop the mysql database), but not the dependencies installed with it on installation. Both commands are equivalent. If you use the default image directory located in the installed directory (/var/lib/dcm4chee/archive), all the images will be deleted.
