#Scrypt For NodeJS
node-scrypt is a native node C++ wrapper for Colin Percival's scrypt [key derivation](http://en.wikipedia.org/wiki/Key_derivation_function) utility. It is fully asynchronous (in fact, there is no synchronous mode due to the time input of scrypt which would block the event loop).

node-scrypt wraps scrypt's C reference implementation and provides implementation for the following:

* Password hash.
* Password hash verification.
* Encryption.
* Decryption.

##What Is Scrypt? 
Scrypt is an advanced crypto library used mainly for [key derivation](http://en.wikipedia.org/wiki/Key_derivation_function) (i.e. password authenticator). More information can be found:

* [Tarsnap blurb about scrypt](http://www.tarsnap.com/scrypt.html) - Colin Percival (the author of scrypt) explains a bit about it.
* [Academic paper explaining scrypt](http://www.tarsnap.com/scrypt/scrypt.pdf).
* [Wikipedia Article on scrypt](http://en.wikipedia.org/wiki/Scrypt).

For additional interest, also read the [key derivation function](http://en.wikipedia.org/wiki/Key_derivation_function) article on wikipedia.

##Why Use Scrypt?
It is probably the most advanced key derivation function available. This is is quote taken from a comment in hacker news:

>Passwords hashed with scrypt with sufficiently-high strength values (there are 3 tweakable input numbers) are fundamentally impervious to being cracked. I use the word "fundamental" in the literal sense, here; even if you had the resources of a large country, you would not be able to design any hardware (whether it be GPU hardware, custom-designed hardware, or otherwise) which could crack these hashes. Ever. (For sufficiently-small definitions of "ever". At the very least "within your lifetime"; probably far longer.)

The *three tweakable* inputs mentioned above are as follows (Quoting from the author):

**maxtime**
>maxtime will instruct scrypt to spend at most maxtime seconds computing the derived encryption key from the password; for encryption, this value will determine how secure the encrypted data is, while for decryption this value is used as an upper limit (if scrypt detects that it would take too long to decrypt the data, it will exit with an error message).

**maxmemfrac**
>maxmemfrac instructs scrypt to use at most the specified fraction of the available RAM for computing the derived encryption key. For encryption, increasing this value might increase the security of the encrypted data, depending on the maxtime value; for decryption, this value is used as an upper limit and may cause scrypt to exit with an error.

**maxmem**
>maxmem instructs scrypt to use at most the specified number of bytes of RAM when computing the derived encryption key. 


**A Note On How Memory Is Calculated**: `maxmem` is often defaulted to `0`. This does not mean that `0` RAM is used. Instead, memory used is calculated like so (quote from Colin Percival):

> the system [will use] the amount of RAM which [is] specified [as the] fraction of the available RAM, but no more than maxmem, and no less than 1MiB

Therefore at the very least, 1MiB of ram will be used.

###The Three Tweakable Inputs
<u>**Note**: This is a very important section to understand</u>. The three tweakable inputs mentioned above are actually just *human understandable* inputs into a translation function that produces the inputs required for the internal scrypt cryptographic function. These inputs (as defined in the [scrypt paper](http://www.tarsnap.com/scrypt/scrypt.pdf)) are as follows:

1. **N** - general work factor, iteration count.
2. **r** - blocksize in use for underlying hash; fine-tunes the relative memory-cost.
3. **p** - parallelization factor; fine-tunes the relative cpu-cost.

Values for *maxtime*, *maxmemfrac* and *maxmem* are translated into the above values, which are then fed to the scrypt function. The translation function also takes into account the CPU and Memory capabilities of a machine. Therefore values of *N*, *r* and *p* may differ for different machines that have different specs.

## Pros And Cons
Here are some pros and cons for using it:

###Pros

* The scrypt algorithm has been published by [IETF](http://en.wikipedia.org/wiki/IETF) as an [Internet Draft](http://en.wikipedia.org/wiki/Internet_Draft) and is thus on track to becoming a standard. See [here](https://tools.ietf.org/html/draft-josefsson-scrypt-kdf-00) for the draft.
* It is being actively used in production at [Tarsnap](http://www.tarsnap.com/).
* It is much more secure than bcrypt.
* It is designed to be future proof against attacks with future (and more advanced) hardware.
* It is designed to defend against large scale custom hardware attacks.
* It is production ready.
* There is a scrypt library for most major scripting languages (Python, Ruby etc). Now this module provides the library for NodeJS :)

I will end this section with a quote from Colin Percival (author of scrypt):

> We estimate that on modern (2009) hardware, if 5 seconds are spent computing a derived key, the cost of a hardware brute-force attack against scrypt is roughly 4000 times greater than the cost of a similar attack against bcrypt (to find the same password), and 20000 times greater than a similar attack against PBKDF2.

###Cons
There is just one con: It is a relatively new library (only been around since 2009). Cryptographers don't really like new libraries for production deployment as it has not been *battle tested*. That being said, it is being actively used in [Tarsnap](http://www.tarsnap.com/) (as mentioned above) and the author is very active.

#Security Issues/Concerns
As should be the case with any security tool, this library should be scrutinized by anyone using it. If you find or suspect an issue with the code- please bring it to my attention and I'll spend some time trying to make sure that this tool is as secure as possible.

#Dependencies
There are no Node module dependencies, but the scrypt C library requires the following:

* Openssl Library - this is linked with `lcrypto` in the makefile (binding.gyp).
* Realtime Extensions Library - linked with `lrt`, used for translation of three tweakable inputs into scrypt inputs (see above for details).

The above libraries are standard on Linux.

#Installation Instructions
As of now (Dec 2012), this library has been tested and works on Linux (Ubuntu to be exact).
##From NPM

    npm install scrypt

##From Source
You will need `node-gyp` to get this to work (install it if you don't have it: `npm install -g node-gyp`):

    git clone https://github.com/barrysteyn/node-scrypt.git
    cd node-scrypt
    node-gyp configure build

#Usage
##Authentication
For interactive authentication, set `maxtime` to `0.1` - 100 milliseconds. 
   
###To create a password hash
 
    var scrypt = require("scrypt");
    var hash;
    var password = "This is a password";
    var maxtime = 0.1;

    scrypt.passwordHash(password, maxtime, function(err, pwdhash) {
        if (!err)
            hash = pwdhash; //This should now be stored in the database
    });

Note `maxmem` and `maxmemfrac` can also be passed to hash function. If they are not passed, then `maxmem` defaults to `0` and `maxmemfrac` defaults to `0.5`. If these values are to be passed, then they must be passed after `maxtime`  and before the callback function like so:
    
    var scrypt = require("scrypt");
    var hash;
    var password = "This is a password";
    var maxtime = 0.1;
    var maxmem = 0, maxmemfrac = 0.5;

    scrypt.passwordHash(password, maxtime, maxmem, maxmemfrac, function(err, pwdhash) {
        if (!err)
            hash = pwdhash; //This should now be stored in the database
    });

###To verify a password hash

    var scrypt = require("scrypt");
    var password = "This is a password";
    var hash; //This should be obtained from the database

    scrypt.verifyHash(hash, password, function(err, result) {
        if (!err)
            return result; //Will be True
        
        return False;    
    });


##Encryption and Decryption
I suspect scrypt will be used mainly as a key derivation function, but I have also ported the scrypt encryption and decryption functions. Performing scrypt cryptography is done if you value security over speed. Scrypt is more secure than a vanilla block cipher (e.g. AES) but it is much slower.

    var scrypt = require("scrypt");
    var message = "Hello World";
    var password = "Pass";
    var maxtime = 1.0;

    scrypt.encrypt(message, password, maxtime, function(err, cipher) {
        console.log(cipher);
        scrypt.decrypt(cipher, password, maxtime, function(err, msg) {
            console.log(msg);
        });
    });

Note that `maxmem` and `maxmemfrac` can also be passed to the functions. If they are not passed, then `maxmem` defaults to `0` and `maxmemfrac` defaults to `0.5`. If these values are to be passed, then they must be passed after `maxtime`  and before the callback function like so:
    
    var scrypt = require("scrypt");
    var message = "Hello World";
    var password = "Pass";
    var maxtime = 1.0;
    var maxmem = 1; //Defaults to 0 if not set
    var maxmemfrac = 1.5; //Defaults to 0.5 if not set

    scrypt.encrypt(message, password, maxtime, maxmem, maxmemfrac, function(err, cipher) {
        console.log(cipher);
        scrypt.decrypt(cipher, password, maxtime, maxmem, maxmemfrac, function(err, msg) {
            console.log(msg);
        });
    });

#Api

##Authentication

* `passwordHash(password, maxtime, maxmem, maxmemfrac, callback_function)`
    * `password` - [REQUIRED] - a password string.
    * `maxtime` - [REQUIRED] - a decimal (double) representing the maxtime in seconds for running scrypt. Use 0.1 (100 milliseconds) for interactive logins.
    * `maxmem` - [OPTIONAL] - instructs scrypt to use the specified number of bytes of RAM (default 0).
    * `maxmemfrac` - [OPTIONAL] - instructs scrypt to use the specified fracion of RAM (defaults 0.5).
    * `callback_function` - [REQUIRED] - a callback function that will handle processing when result is ready.
* `verifyHash(hash, password, callback_function)` 
    * `hash` - [REQUIRED] - the password created with the above `passwordHash` function.
    * `password` - [REQUIRED] - a password string.
    * `callback_function` - [REQUIRED] - a callback function that will handle processing when result is ready.
           
##Encryption/Decryption

* `encrypt(message, password, maxtime, maxmem, maxmemfrac, callback_function)`
    * `message` - [REQUIRED] - the message data to be encrypted.
    * `password` - [REQUIRED] - a password string.
    * `maxtime` - [REQUIRED] - a decimal (double) representing the maxtime in seconds for running scrypt.
    * `maxmem` - [OPTIONAL] - instructs scrypt to use the specified number of bytes of RAM (default 0).
    * `maxmemfrac` - [OPTIONAL] - instructs scrypt to use the specified fracion of RAM (defaults 0.5).
    * `callback_function` - [REQUIRED] - a callback function that will handle processing when result is ready.
* `decrypt(cipher, password, maxtime, maxmem, maxmemfrac, callback_function)`
    * `cipher` - [REQUIRED] - the cipher to be decrypted.
    * `password` - [REQUIRED] - a password string.
    * `maxtime` - [REQUIRED] - a decimal (double) representing the maxtime in seconds for running scrypt.
    * `maxmem` - [OPTIONAL] - instructs scrypt to use the specified number of bytes of RAM (default 0).
    * `maxmemfrac` - [OPTIONAL] - instructs scrypt to use the specified fracion of RAM (defaults 0.5).
    * `callback_function` - [REQUIRED] - a callback function that will handle processing when result is ready.

#Credits
The scrypt library is Colin Percival's [scrypt](http://www.tarsnap.com/scrypt.html) project. This includes the encryption/decryption functions which are basically just wrappers into this library.

The password hash and verify functions are also very heavily influenced by the scrypt source code, with most functionality being copied from various placed within scrypt.

#Contributors

* [René Nyffenegger](http://www.adp-gmbh.ch/) - produced original Base64 encoding code.

#A Call For Help

I need help with the following please:

1. Porting this to Windows and Mac.
2. Scrutiny by cryptographers.
3. Testing against bcrypt (from the scrypt paper, Colin Percival says it is much more secure, and provides proof - but I would like a back up of that fact).
