If we are being honest, I'm more of a low level person. I enjoy understanding things from the bottom up, and don't like taking shortcuts.

If I was going to switch to Rust, I'd ideally like to do initial board bringup/firmware development in Rust on a devkit so I'd actually know what was happening. It's only supported on a few specific pieces of hardware right now. There are a few hardware architectures I'd like to get a crack at (whether FPGA, embedded linux, baremetal, FreeRTOS scheduler, or Something Else TM).

In real life, one person usually does that work for a long time after hardware folks set it up, and then sets up a hardware abstraction layer and tutorial to bring on maybe one other person, and it takes a significant amount of time before they are ready to actually support software folks iterating on it, and even then it needs to be slow. For an actual realtime system (or a microcontroller) I'm not really sure it ever makes sense to have software-only folks.

It's scary to trust a person to do that - unless they have done it once (or twice) before, are good at working with avionics folks, and know what the consequences are for everyone if that information stays gatekept later. Especially for a microcontroller. They need to be shielded in the beginning to train themselves with tutorials on a devkit, or secretly start early and bring themselves up on the correct devkit.

I know this job is probably meant to support things that already exist. Just saying.
