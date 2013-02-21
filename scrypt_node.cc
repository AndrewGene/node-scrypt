/*
scrypt_node.cc

Copyright (C) 2013 Barry Steyn (http://doctrina.org/Scrypt-Authentication-For-Node.html)

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.

3. This notice may not be removed or altered from any source distribution.

Barry Steyn barry.steyn@gmail.com

*/

#include <node.h>
#include <v8.h>

using namespace v8;

/*
 * Forward declarations
 */
Handle<Value> HashAsyncBefore(const Arguments& args);
Handle<Value> VerifyAsyncBefore(const Arguments& args);
Handle<Value> EncryptAsyncBefore(const Arguments& args);
Handle<Value> DecryptAsyncBefore(const Arguments& args);

/*
 * Module initialisation function
 */
void RegisterModule(Handle<Object> target) {
    //Asynchronous
    target->Set(String::NewSymbol("passwordHash"),
        FunctionTemplate::New(HashAsyncBefore)->GetFunction());

    target->Set(String::NewSymbol("verifyHash"),
        FunctionTemplate::New(VerifyAsyncBefore)->GetFunction());

    target->Set(String::NewSymbol("encrypt"),
        FunctionTemplate::New(EncryptAsyncBefore)->GetFunction());

    target->Set(String::NewSymbol("decrypt"),
        FunctionTemplate::New(DecryptAsyncBefore)->GetFunction());
}
NODE_MODULE(scrypt, RegisterModule)
