## DX7GEN

Ever wanted your DX7 to sound like it's terminally ill? Easily generate random DX7 cartridges with dx7gen!  
Every patch and every parameter is completely random, so this is a great way to make wierd and complex sounds without any knowledge of fm synthesis.

Be warned, although many of the patches will turn out incredibly quiet or even completely silent, others will not, and might very well ruin your eardrums and/or equipment if you turned the volume up too high.
I would recommend using a compressor or two to make the quiet sounds more audible, and a limiter to protect your ears and speakers! Use with caution, and don't turn your volume up louder than you normally would.

dx7gen generates a Yamaha SysEx file.
Only tested with [dexed](https://asb2m10.github.io/dexed/), but should work on a DX7. (I would love to know for sure, if anyone has the ability to test)


## Compile and use

To compile,  simply run `gcc dx7gen.c -o dx7gen`

To generate a SysEx file called *foo.syx*, run `./dx7gen foo`  
Note that you don't have to specify the extension, as dx7gen will do that for you.

To load your fancy new cartridge in dexed, open dexed and press '*CART*' then '*LOAD*'. Locate your SysEx file, select it and press '*Open*'.
Alternatively, you can copy the file into dexed's '*Cartridges*' directory. You can locate this directory by pressing '*CART*' then '*SHOW DIR*'.


## Features

dx7gen is extremely barebones and doesn't have any real features.
I am thinking of adding a few ones, namely the ability to specify parameter ranges and values, as well as seeds. If you have ideas for features you would like to see implemented, I would love to hear about them!
If you want to help impelement a feature or two, that would be great aswell!
