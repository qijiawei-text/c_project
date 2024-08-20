# ETSI NFV-SOL 006 v2.7.1 - YANG models for NFV descriptors

This repository consists of YANG models for representing Network Functions Virtualisation (NFV)
descriptors, fulfilling the requirements specified in ETSI GS NFV-IFA 011 and 
ETSI GS NFV-IFA 014 applicable to a Virtualised Network Function Descriptor (VNFD), 
a Physical Network Functions Descriptor (PNFD) and a Network Service Descriptor (NSD).

The example-data folder consists of instance data files that serve as examples for the model. 

A more detailed explanation of the models and the examples can be found in [ETSI GS NFV-SOL 006](https://portal.etsi.org/webapp/WorkProgram/Report_WorkItem.asp?WKI_ID=57489).

## Repository organization

Given the versioning conventions of ISG NFV, **only the first two digits** of the version are increased in subsequent releases (e.g. `2.6.1`, `2.7.1`, `2.8.1`, `3.1.1`, ...).

In order to keep alignment between the versions of the repository and the versions of ETSI GS NFV-SOL 006, the following approach was agreed:

* When a new revision `A.B.C` of SOL 006 is initiated a new branch `dev-vA.B.C` is created on top of the previously published branch;
* The README gets modified in order to show the new target version and to indicate that the content is still in the drafting phase;
* When the content is to be published, a new branch `vA.B.C` is created and the branch `dev-vA.B.C` is merged in it;
* When the document is published the branch `vA.B.C` gets frozen and no more contributions are allowed. 

## How to contribute

ETSI Forge uses Gitlab to manage submissions to the repository. 
Check the project page [here](https://forge.etsi.org/rep/nfv/SOL006) to submit contributions or to file requests and issues.

For more information on setting up your environment and contributing, you may 
refer to the [ETSI Forge wiki](https://forge.etsi.org/wiki/index.php/Main_Page).

