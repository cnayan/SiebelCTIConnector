Siebel Call Center Connector
============================

SCAPI implementation for a Call Center Communications Driver, to connect it to a PBX.

This Windows based sample DLL (Siebel CTI driver) implements the SCAPI method
declarations as described by Siebel CTI Administration Guide's section
"Developing a Communications Driver".

**Motivation**

This may be too late, as there are many professional products already available
in market, but integrating with Siebel has been a black box area, and I mean to
simplify/demystify it.

**Requirements**

Software required (and tested on "Siebel Innovation Pack 2013" with):
* IIS for Windows 2012 Server
* MS SQL Server 2012 Express Edition
* Siebel Gateway 8.2
* Siebel Server 8.2
* Siebel Webserver Extension 8.2
* DevC++ with atleast GCC 4.9.2 (for me it was DevC++ 5.11, TDM-GCC 4.9.2)

**Preparation**

You need to install a VirtualBox VM with Windows 2012 and SQL Server and IIS,
with minimum 4 GB Memory, 1 CPU (more is always better). Install Siebel on it
and configure it. This is the most tedious part. I suggest you hire a
professional to do this.

Or, you may go through the single-click setup of Siebel, created for CentOs.
Visit https://github.com/henkwiedig/SiebelInstaller for details. I have not
used it myself, but it looks good to be tried.

Also, you need to configure some login IDs - one be a Siebel administrator,
others can be Call Center agents. Without it, you will not be able to test
the driver.

**Building the code**

The code is in S_CTI folder. Open the S_CTI.dev file in DevC++, and build it
in 32-bit release mode. I have found that Siebel application that loads the DLL
in memory, is actually 32 bit. So, it won't be useful to build 64 bit DLL.

Note that I have used C++11, hence you must enable the support for C++11 code
compilation via project options. At the time of writing this code, it is in
experimental stage, but worked fine.

**What is DEF file?**

S_CTI.DEF is a Siebel exported definition file, that contains a sample
Communications Driver template that I have used. I have configured it slightly
to be used via the DLL code. You need to import this file in to Siebel, before
you can use the DLL. Make sure that the "Library Name" in the DEF is pointing
to the exact path of the DLL, or if you just keep it as is, it will assume that
the DLL is present in the {siebel installation directory}\ses\siebsrvr\bin
folder. You can change it from within Siebel as well, when you log in as siebel
administrator, from the "Communications Drivers And Profiles" section.

**Developer Documents**

* Siebel CTI Administration Guide (https://docs.oracle.com/cd/E14004_01/books/SiebCTIAdm/booktitle.html or search Oracle site for its latest version.)

**Understanding the SCAPI and flow**

I will update the guide here, very soon.

**TODO**

Since this is just the bare bone implementation of SCAPI methods, there is
lot to be done to make it fully functional. The driver is able to take commands
and events from Siebel, and pass it some too. But the complete logic of
handling a voice-call/email/chat flow is not implemented here.
