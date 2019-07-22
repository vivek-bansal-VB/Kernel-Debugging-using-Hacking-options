# Kernel-Debugging-using-Hacking-options
* PROJECT DESCRIPTION:

The Linux "kernel hacking" section lists several dozen configuration options
that help you catch various bugs.  Select ten (10) such configuration
options and enable them in your kernel (if it helps your work, you can
enable just the relevant features and install multiple kernels in your VMs).
For each such configuration option, write kernel code to demonstrate how the
kernel hacking option catches and reports the bugs(s).  Yes, this assignment
asks you to write BUGGY code (but don't assume it's easier to write bad
code).  You can write the code as a standalone module (preferably one module
per demonstrated bug), or integrate it into the kernel, but in such a way
that you can easily trigger the code for bug 1, bug 2, etc.  Note that I'd
like you to get to the essence of the bug with as few lines of code as
possible, so don't write too much code if the same bug can be demonstrated
with fewer lines of code (but you can have as many printk's as you'd like).

You can select any 10 configuration options with the following restrictions.

First, the kernel hacking features you pick should be those that dynamically
(e.g., at kernel runtime) detect the bug, not merely informational and/or
statistical features.

Second, try to select features from different "classes" of bugs (e.g., avoid
selecting all your 10 cases from the "lock debugging" section).

Third, we will judge your HW3 by how difficult the bug is to trigger and how
your code demonstrates the bug most effectively and cleanly.
