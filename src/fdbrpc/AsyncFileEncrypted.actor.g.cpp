#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
/*
 * AsyncFileEncrypted.actor.cpp
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fdbrpc/AsyncFileEncrypted.h"
#include "flow/StreamCipher.h"
#include "flow/UnitTest.h"
#include "xxhash.h"
#include "flow/actorcompiler.h" // must be last include

class AsyncFileEncryptedImpl {
public:
	// Determine the initialization for the first block of a file based on a hash of
	// the filename.
	static auto getFirstBlockIV(const std::string& filename) {
		StreamCipher::IV iv;
		auto salt = basename(filename);
		auto pos = salt.find('.');
		salt = salt.substr(0, pos);
		auto hash = XXH3_128bits(salt.c_str(), salt.size());
		auto pHigh = reinterpret_cast<unsigned char*>(&hash.high64);
		auto pLow = reinterpret_cast<unsigned char*>(&hash.low64);
		std::copy(pHigh, pHigh + 8, &iv[0]);
		std::copy(pLow, pLow + 4, &iv[8]);
		uint32_t blockZero = 0;
		auto pBlock = reinterpret_cast<unsigned char*>(&blockZero);
		std::copy(pBlock, pBlock + 4, &iv[12]);
		return iv;
	}

	// Read a single block of size ENCRYPTION_BLOCK_SIZE bytes, and decrypt.
																#line 50 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
// This generated class is to be used only via readBlock()
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class ReadBlockActor>
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ReadBlockActorState {
															#line 56 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ReadBlockActorState(AsyncFileEncrypted* const& self,uint32_t const& block) 
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : self(self),
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   block(block),
															#line 49 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   arena(),
															#line 50 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   encrypted(new (arena) unsigned char[FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE])
															#line 69 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("readBlock", reinterpret_cast<unsigned long>(this));

	}
	~ReadBlockActorState() 
	{
		fdb_probe_actor_destroy("readBlock", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 51 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<int> __when_expr_0 = uncancellable(holdWhile(arena, self->file->read(encrypted, FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE, FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE * block)));
															#line 51 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<ReadBlockActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 86 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadBlockActor*>(this)->actor_wait_state = 1;
															#line 51 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadBlockActor, 0, int >*>(static_cast<ReadBlockActor*>(this)));
															#line 91 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ReadBlockActorState();
		static_cast<ReadBlockActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& bytes,int loopDepth) 
	{
															#line 55 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StreamCipherKey const* cipherKey = StreamCipherKey::getGlobalCipherKey();
															#line 56 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		DecryptionStreamCipher decryptor(cipherKey, self->getIV(block));
															#line 57 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto decrypted = decryptor.decrypt(encrypted, bytes, arena);
															#line 58 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<ReadBlockActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(decrypted, arena)); this->~ReadBlockActorState(); static_cast<ReadBlockActor*>(this)->destroy(); return 0; }
															#line 120 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<ReadBlockActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Standalone<StringRef>(decrypted, arena));
		this->~ReadBlockActorState();
		static_cast<ReadBlockActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && bytes,int loopDepth) 
	{
															#line 55 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StreamCipherKey const* cipherKey = StreamCipherKey::getGlobalCipherKey();
															#line 56 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		DecryptionStreamCipher decryptor(cipherKey, self->getIV(block));
															#line 57 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto decrypted = decryptor.decrypt(encrypted, bytes, arena);
															#line 58 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<ReadBlockActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(decrypted, arena)); this->~ReadBlockActorState(); static_cast<ReadBlockActor*>(this)->destroy(); return 0; }
															#line 138 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<ReadBlockActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Standalone<StringRef>(decrypted, arena));
		this->~ReadBlockActorState();
		static_cast<ReadBlockActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& bytes,int loopDepth) 
	{
		loopDepth = a_body1cont1(bytes, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && bytes,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(bytes), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadBlockActor*>(this)->actor_wait_state > 0) static_cast<ReadBlockActor*>(this)->actor_wait_state = 0;
		static_cast<ReadBlockActor*>(this)->ActorCallback< ReadBlockActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadBlockActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("readBlock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadBlockActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("readBlock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadBlockActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("readBlock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlock", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	AsyncFileEncrypted* self;
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	uint32_t block;
															#line 49 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Arena arena;
															#line 50 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	unsigned char* encrypted;
															#line 217 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via readBlock()
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ReadBlockActor final : public Actor<Standalone<StringRef>>, public ActorCallback< ReadBlockActor, 0, int >, public FastAllocated<ReadBlockActor>, public ReadBlockActorState<ReadBlockActor> {
															#line 222 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<ReadBlockActor>::operator new;
	using FastAllocated<ReadBlockActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<StringRef>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadBlockActor, 0, int >;
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ReadBlockActor(AsyncFileEncrypted* const& self,uint32_t const& block) 
															#line 233 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<Standalone<StringRef>>(),
		   ReadBlockActorState<ReadBlockActor>(self, block)
	{
		fdb_probe_actor_enter("readBlock", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readBlock");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readBlock", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadBlockActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
[[nodiscard]] static Future<Standalone<StringRef>> readBlock( AsyncFileEncrypted* const& self, uint32_t const& block ) {
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<Standalone<StringRef>>(new ReadBlockActor(self, block));
															#line 260 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}

#line 60 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"

																#line 265 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
// This generated class is to be used only via read()
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class ReadActor>
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ReadActorState {
															#line 271 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ReadActorState(Reference<AsyncFileEncrypted> const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : self(self),
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   data(data),
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   length(length),
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   offset(offset),
															#line 62 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   firstBlock(offset / FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE),
															#line 63 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   lastBlock((offset + length - 1) / FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE),
															#line 64 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   block(),
															#line 65 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   output(reinterpret_cast<unsigned char*>(data)),
															#line 66 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   bytesRead(0)
															#line 294 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("read", reinterpret_cast<unsigned long>(this));

	}
	~ReadActorState() 
	{
		fdb_probe_actor_destroy("read", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 67 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT(self->mode == AsyncFileEncrypted::Mode::READ_ONLY);
															#line 68 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			block = firstBlock;
															#line 311 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = a_body1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 98 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<ReadActor*>(this)->SAV<int>::futures) { (void)(bytesRead); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 334 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<ReadActor*>(this)->SAV< int >::value()) int(std::move(bytesRead)); // state_var_RVO
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 68 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!(block <= lastBlock))
															#line 353 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 69 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		plaintext = Standalone<StringRef>();
															#line 71 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto cachedBlock = self->readBuffers.get(block);
															#line 72 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (cachedBlock.present())
															#line 363 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
															#line 73 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			plaintext = cachedBlock.get();
															#line 367 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		else
		{
															#line 75 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<Void> __when_expr_0 = store(plaintext, readBlock(self.getPtr(), block));
															#line 75 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 376 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadActor*>(this)->actor_wait_state = 1;
															#line 75 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 0, Void >*>(static_cast<ReadActor*>(this)));
															#line 381 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto start = (block == firstBlock) ? plaintext.begin() + (offset % FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE) : plaintext.begin();
															#line 80 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto end = (block == lastBlock) ? plaintext.begin() + ((offset + length) % FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE) : plaintext.end();
															#line 83 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if ((offset + length) % FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE == 0)
															#line 408 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
															#line 84 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			end = plaintext.end();
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		}
															#line 88 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		end = std::min(end, plaintext.end());
															#line 90 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (start == end || start >= plaintext.end())
															#line 418 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 94 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		std::copy(start, end, output);
															#line 95 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		output += (end - start);
															#line 96 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesRead += (end - start);
															#line 68 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		++block;
															#line 430 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->readBuffers.insert(block, plaintext);
															#line 439 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->readBuffers.insert(block, plaintext);
															#line 448 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Reference<AsyncFileEncrypted> self;
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	void* data;
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int length;
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int64_t offset;
															#line 62 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	const uint32_t firstBlock;
															#line 63 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	const uint32_t lastBlock;
															#line 64 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	uint32_t block;
															#line 65 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	unsigned char* output;
															#line 66 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int bytesRead;
															#line 69 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Standalone<StringRef> plaintext;
															#line 536 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via read()
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ReadActor final : public Actor<int>, public ActorCallback< ReadActor, 0, Void >, public FastAllocated<ReadActor>, public ReadActorState<ReadActor> {
															#line 541 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<ReadActor>::operator new;
	using FastAllocated<ReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadActor, 0, Void >;
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ReadActor(Reference<AsyncFileEncrypted> const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 552 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<int>(),
		   ReadActorState<ReadActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
[[nodiscard]] static Future<int> read( Reference<AsyncFileEncrypted> const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<int>(new ReadActor(self, data, length, offset));
															#line 579 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}

#line 100 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"

																#line 584 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
// This generated class is to be used only via write()
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class WriteActor>
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class WriteActorState {
															#line 590 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	WriteActorState(Reference<AsyncFileEncrypted> const& self,void const* const& data,int const& length,int64_t const& offset) 
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : self(self),
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   data(data),
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   length(length),
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   offset(offset)
															#line 603 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("write", reinterpret_cast<unsigned long>(this));

	}
	~WriteActorState() 
	{
		fdb_probe_actor_destroy("write", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 102 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT(self->mode == AsyncFileEncrypted::Mode::APPEND_ONLY);
															#line 104 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT_EQ(offset, self->currentBlock * FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE + self->offsetInBlock);
															#line 105 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			input = reinterpret_cast<unsigned char const*>(data);
															#line 106 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			;
															#line 624 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = a_body1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 124 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<WriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteActorState(); static_cast<WriteActor*>(this)->destroy(); return 0; }
															#line 647 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<WriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 106 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!(length > 0))
															#line 666 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 107 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		const auto chunkSize = std::min(length, FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE - self->offsetInBlock);
															#line 108 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		Arena arena;
															#line 109 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		auto encrypted = self->encryptor->encrypt(input, chunkSize, arena);
															#line 110 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		std::copy(encrypted.begin(), encrypted.end(), &self->writeBuffer[self->offsetInBlock]);
															#line 111 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		offset += encrypted.size();
															#line 112 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->offsetInBlock += chunkSize;
															#line 113 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		length -= chunkSize;
															#line 114 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		input += chunkSize;
															#line 115 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (self->offsetInBlock == FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE)
															#line 688 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
															#line 116 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->writeLastBlockToFile();
															#line 116 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 694 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 1;
															#line 116 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 0, Void >*>(static_cast<WriteActor*>(this)));
															#line 699 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 117 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->offsetInBlock = 0;
															#line 118 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		ASSERT_LT(self->currentBlock, std::numeric_limits<uint32_t>::max());
															#line 119 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		++self->currentBlock;
															#line 120 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->encryptor = std::make_unique<EncryptionStreamCipher>(StreamCipherKey::getGlobalCipherKey(), self->getIV(self->currentBlock));
															#line 738 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 117 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->offsetInBlock = 0;
															#line 118 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		ASSERT_LT(self->currentBlock, std::numeric_limits<uint32_t>::max());
															#line 119 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		++self->currentBlock;
															#line 120 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		self->encryptor = std::make_unique<EncryptionStreamCipher>(StreamCipherKey::getGlobalCipherKey(), self->getIV(self->currentBlock));
															#line 753 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Reference<AsyncFileEncrypted> self;
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	void const* data;
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int length;
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int64_t offset;
															#line 105 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	unsigned char const* input;
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via write()
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class WriteActor final : public Actor<Void>, public ActorCallback< WriteActor, 0, Void >, public FastAllocated<WriteActor>, public WriteActorState<WriteActor> {
															#line 836 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<WriteActor>::operator new;
	using FastAllocated<WriteActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteActor, 0, Void >;
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	WriteActor(Reference<AsyncFileEncrypted> const& self,void const* const& data,int const& length,int64_t const& offset) 
															#line 847 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<Void>(),
		   WriteActorState<WriteActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
[[nodiscard]] static Future<Void> write( Reference<AsyncFileEncrypted> const& self, void const* const& data, int const& length, int64_t const& offset ) {
															#line 101 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<Void>(new WriteActor(self, data, length, offset));
															#line 874 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}

#line 126 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"

																#line 879 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
// This generated class is to be used only via sync()
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class SyncActor>
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class SyncActorState {
															#line 885 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	SyncActorState(Reference<AsyncFileEncrypted> const& self) 
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : self(self)
															#line 892 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("sync", reinterpret_cast<unsigned long>(this));

	}
	~SyncActorState() 
	{
		fdb_probe_actor_destroy("sync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 128 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT(self->mode == AsyncFileEncrypted::Mode::APPEND_ONLY);
															#line 129 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->writeLastBlockToFile();
															#line 129 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 911 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 1;
															#line 129 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 0, Void >*>(static_cast<SyncActor*>(this)));
															#line 916 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->file->sync();
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 941 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 946 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->file->sync();
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 957 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 962 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 131 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 1034 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 131 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 1046 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Reference<AsyncFileEncrypted> self;
															#line 1119 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via sync()
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class SyncActor final : public Actor<Void>, public ActorCallback< SyncActor, 0, Void >, public ActorCallback< SyncActor, 1, Void >, public FastAllocated<SyncActor>, public SyncActorState<SyncActor> {
															#line 1124 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<SyncActor>::operator new;
	using FastAllocated<SyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SyncActor, 0, Void >;
friend struct ActorCallback< SyncActor, 1, Void >;
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	SyncActor(Reference<AsyncFileEncrypted> const& self) 
															#line 1136 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<Void>(),
		   SyncActorState<SyncActor>(self)
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SyncActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
[[nodiscard]] static Future<Void> sync( Reference<AsyncFileEncrypted> const& self ) {
															#line 127 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<Void>(new SyncActor(self));
															#line 1164 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}

#line 133 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"

																#line 1169 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
// This generated class is to be used only via zeroRange()
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class ZeroRangeActor>
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ZeroRangeActorState {
															#line 1175 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ZeroRangeActorState(AsyncFileEncrypted* const& self,int64_t const& offset,int64_t const& length) 
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : self(self),
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   offset(offset),
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   length(length)
															#line 1186 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("zeroRange", reinterpret_cast<unsigned long>(this));

	}
	~ZeroRangeActorState() 
	{
		fdb_probe_actor_destroy("zeroRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 135 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT(self->mode == AsyncFileEncrypted::Mode::APPEND_ONLY);
															#line 137 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			Arena arena;
															#line 138 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			auto zeroes = new (arena) unsigned char[length];
															#line 139 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			memset(zeroes, 0, length);
															#line 140 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<Void> __when_expr_0 = uncancellable(holdWhile(arena, self->write(zeroes, length, offset)));
															#line 140 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<ZeroRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1211 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ZeroRangeActor*>(this)->actor_wait_state = 1;
															#line 140 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ZeroRangeActor, 0, Void >*>(static_cast<ZeroRangeActor*>(this)));
															#line 1216 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ZeroRangeActorState();
		static_cast<ZeroRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 141 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<ZeroRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ZeroRangeActorState(); static_cast<ZeroRangeActor*>(this)->destroy(); return 0; }
															#line 1239 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<ZeroRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ZeroRangeActorState();
		static_cast<ZeroRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 141 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<ZeroRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ZeroRangeActorState(); static_cast<ZeroRangeActor*>(this)->destroy(); return 0; }
															#line 1251 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<ZeroRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ZeroRangeActorState();
		static_cast<ZeroRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ZeroRangeActor*>(this)->actor_wait_state > 0) static_cast<ZeroRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ZeroRangeActor*>(this)->ActorCallback< ZeroRangeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ZeroRangeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("zeroRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ZeroRangeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("zeroRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ZeroRangeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("zeroRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("zeroRange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	AsyncFileEncrypted* self;
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int64_t offset;
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int64_t length;
															#line 1328 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via zeroRange()
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class ZeroRangeActor final : public Actor<Void>, public ActorCallback< ZeroRangeActor, 0, Void >, public FastAllocated<ZeroRangeActor>, public ZeroRangeActorState<ZeroRangeActor> {
															#line 1333 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<ZeroRangeActor>::operator new;
	using FastAllocated<ZeroRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ZeroRangeActor, 0, Void >;
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	ZeroRangeActor(AsyncFileEncrypted* const& self,int64_t const& offset,int64_t const& length) 
															#line 1344 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<Void>(),
		   ZeroRangeActorState<ZeroRangeActor>(self, offset, length)
	{
		fdb_probe_actor_enter("zeroRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("zeroRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("zeroRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ZeroRangeActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
[[nodiscard]] static Future<Void> zeroRange( AsyncFileEncrypted* const& self, int64_t const& offset, int64_t const& length ) {
															#line 134 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<Void>(new ZeroRangeActor(self, offset, length));
															#line 1371 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}

#line 143 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
};

AsyncFileEncrypted::AsyncFileEncrypted(Reference<IAsyncFile> file, Mode mode)
  : file(file), mode(mode), readBuffers(FLOW_KNOBS->MAX_DECRYPTED_BLOCKS), currentBlock(0) {
	firstBlockIV = AsyncFileEncryptedImpl::getFirstBlockIV(file->getFilename());
	if (mode == Mode::APPEND_ONLY) {
		encryptor =
		    std::make_unique<EncryptionStreamCipher>(StreamCipherKey::getGlobalCipherKey(), getIV(currentBlock));
		writeBuffer = std::vector<unsigned char>(FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE, 0);
	}
}

void AsyncFileEncrypted::addref() {
	ReferenceCounted<AsyncFileEncrypted>::addref();
}

void AsyncFileEncrypted::delref() {
	ReferenceCounted<AsyncFileEncrypted>::delref();
}

Future<int> AsyncFileEncrypted::read(void* data, int length, int64_t offset) {
	return AsyncFileEncryptedImpl::read(Reference<AsyncFileEncrypted>::addRef(this), data, length, offset);
}

Future<Void> AsyncFileEncrypted::write(void const* data, int length, int64_t offset) {
	return AsyncFileEncryptedImpl::write(Reference<AsyncFileEncrypted>::addRef(this), data, length, offset);
}

Future<Void> AsyncFileEncrypted::zeroRange(int64_t offset, int64_t length) {
	return AsyncFileEncryptedImpl::zeroRange(this, offset, length);
}

Future<Void> AsyncFileEncrypted::truncate(int64_t size) {
	ASSERT(mode == Mode::APPEND_ONLY);
	return file->truncate(size);
}

Future<Void> AsyncFileEncrypted::sync() {
	ASSERT(mode == Mode::APPEND_ONLY);
	return AsyncFileEncryptedImpl::sync(Reference<AsyncFileEncrypted>::addRef(this));
}

Future<Void> AsyncFileEncrypted::flush() {
	ASSERT(mode == Mode::APPEND_ONLY);
	return Void();
}

Future<int64_t> AsyncFileEncrypted::size() const {
	ASSERT(mode == Mode::READ_ONLY);
	return file->size();
}

std::string AsyncFileEncrypted::getFilename() const {
	return file->getFilename();
}

Future<Void> AsyncFileEncrypted::readZeroCopy(void** data, int* length, int64_t offset) {
	throw io_error();
	return Void();
}

void AsyncFileEncrypted::releaseZeroCopy(void* data, int length, int64_t offset) {
	throw io_error();
}

int64_t AsyncFileEncrypted::debugFD() const {
	return file->debugFD();
}

StreamCipher::IV AsyncFileEncrypted::getIV(uint32_t block) const {
	auto iv = firstBlockIV;

	auto pBlock = reinterpret_cast<unsigned char*>(&block);
	std::copy(pBlock, pBlock + 4, &iv[12]);

	return iv;
}

Future<Void> AsyncFileEncrypted::writeLastBlockToFile() {
	// The source buffer for the write is owned by *this so this must be kept alive by reference count until the write
	// is finished.
	return uncancellable(
	    holdWhile(Reference<AsyncFileEncrypted>::addRef(this),
	              file->write(&writeBuffer[0], offsetInBlock, currentBlock * FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE)));
}

size_t AsyncFileEncrypted::RandomCache::evict() {
	ASSERT_EQ(vec.size(), maxSize);
	auto index = deterministicRandom()->randomInt(0, maxSize);
	hashMap.erase(vec[index]);
	return index;
}

AsyncFileEncrypted::RandomCache::RandomCache(size_t maxSize) : maxSize(maxSize) {
	vec.reserve(maxSize);
}

void AsyncFileEncrypted::RandomCache::insert(uint32_t block, const Standalone<StringRef>& value) {
	auto [_, found] = hashMap.insert({ block, value });
	if (found) {
		return;
	} else if (vec.size() < maxSize) {
		vec.push_back(block);
	} else {
		auto index = evict();
		vec[index] = block;
	}
}

Optional<Standalone<StringRef>> AsyncFileEncrypted::RandomCache::get(uint32_t block) const {
	auto it = hashMap.find(block);
	if (it == hashMap.end()) {
		return {};
	} else {
		return it->second;
	}
}

// This test writes random data into an encrypted file in random increments,
// then reads this data back from the file in random increments, then confirms that
// the bytes read match the bytes written.
															#line 1496 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase264()
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
template <class FlowTestCase264Actor>
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class FlowTestCase264ActorState {
															#line 1503 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	FlowTestCase264ActorState(UnitTestParameters const& params) 
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		 : params(params),
															#line 265 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   bytes(FLOW_KNOBS->ENCRYPTION_BLOCK_SIZE * deterministicRandom()->randomInt(0, 1000)),
															#line 266 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		   writeBuffer(bytes, 0)
															#line 1514 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase264", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase264ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase264", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			generateRandomData(&writeBuffer.front(), bytes);
															#line 268 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			readBuffer = std::vector<unsigned char>(bytes, 0);
															#line 269 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			ASSERT(g_network->isSimulated());
															#line 270 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StreamCipherKey::initializeGlobalRandomTestKey();
															#line 271 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			int flags = IAsyncFile::OPEN_READWRITE | IAsyncFile::OPEN_CREATE | IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_UNBUFFERED | IAsyncFile::OPEN_ENCRYPTED | IAsyncFile::OPEN_UNCACHED | IAsyncFile::OPEN_NO_AIO;
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open(joinPath(params.getDataDir(), "test-encrypted-file"), flags, 0600);
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1541 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 1;
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >*>(static_cast<FlowTestCase264Actor*>(this)));
															#line 1546 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~FlowTestCase264ActorState();
		static_cast<FlowTestCase264Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 276 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesWritten = 0;
															#line 277 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		chunkSize = int();
															#line 278 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		;
															#line 1573 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __file,int loopDepth) 
	{
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		file = __file;
															#line 1582 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __file,int loopDepth) 
	{
		file = std::move(__file);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase264Actor*>(this)->ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 283 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StrictFuture<Void> __when_expr_2 = file->sync();
															#line 283 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1651 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 3;
															#line 283 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase264Actor, 2, Void >*>(static_cast<FlowTestCase264Actor*>(this)));
															#line 1656 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 278 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!(bytesWritten < bytes))
															#line 1672 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 279 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		chunkSize = std::min(deterministicRandom()->randomInt(0, 100), bytes - bytesWritten);
															#line 280 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->write(&writeBuffer[bytesWritten], chunkSize, bytesWritten);
															#line 280 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1682 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 2;
															#line 280 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase264Actor, 1, Void >*>(static_cast<FlowTestCase264Actor*>(this)));
															#line 1687 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 281 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesWritten += chunkSize;
															#line 1709 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 281 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesWritten += chunkSize;
															#line 1718 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase264Actor*>(this)->ActorCallback< FlowTestCase264Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase264Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 284 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesRead = 0;
															#line 285 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		;
															#line 1792 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1cont3loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 284 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesRead = 0;
															#line 285 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		;
															#line 1803 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = a_body1cont3loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase264Actor*>(this)->ActorCallback< FlowTestCase264Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase264Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 291 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		ASSERT(writeBuffer == readBuffer);
															#line 292 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!static_cast<FlowTestCase264Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase264ActorState(); static_cast<FlowTestCase264Actor*>(this)->destroy(); return 0; }
															#line 1877 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		new (&static_cast<FlowTestCase264Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase264ActorState();
		static_cast<FlowTestCase264Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont3loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3loopBody1(int loopDepth) 
	{
															#line 285 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (!(bytesRead < bytes))
															#line 1896 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		{
			return a_body1cont3break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 286 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		chunkSize = std::min(deterministicRandom()->randomInt(0, 100), bytes - bytesRead);
															#line 287 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		StrictFuture<int> __when_expr_3 = file->read(&readBuffer[bytesRead], chunkSize, bytesRead);
															#line 287 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1906 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont3loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 4;
															#line 287 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase264Actor, 3, int >*>(static_cast<FlowTestCase264Actor*>(this)));
															#line 1911 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3break1(int loopDepth) 
	{
		try {
			return a_body1cont4(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3loopBody1cont1(int const& bytesReadInChunk,int loopDepth) 
	{
															#line 288 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		ASSERT_EQ(bytesReadInChunk, chunkSize);
															#line 289 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesRead += bytesReadInChunk;
															#line 1935 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont3loopHead1(0);

		return loopDepth;
	}
	int a_body1cont3loopBody1cont1(int && bytesReadInChunk,int loopDepth) 
	{
															#line 288 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		ASSERT_EQ(bytesReadInChunk, chunkSize);
															#line 289 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
		bytesRead += bytesReadInChunk;
															#line 1946 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont3loopHead1(0);

		return loopDepth;
	}
	int a_body1cont3loopBody1when1(int const& bytesReadInChunk,int loopDepth) 
	{
		loopDepth = a_body1cont3loopBody1cont1(bytesReadInChunk, loopDepth);

		return loopDepth;
	}
	int a_body1cont3loopBody1when1(int && bytesReadInChunk,int loopDepth) 
	{
		loopDepth = a_body1cont3loopBody1cont1(std::move(bytesReadInChunk), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase264Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase264Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase264Actor*>(this)->ActorCallback< FlowTestCase264Actor, 3, int >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 3, int >*,int const& value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase264Actor, 3, int >*,int && value) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase264Actor, 3, int >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	UnitTestParameters params;
															#line 265 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	const int bytes;
															#line 266 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	std::vector<unsigned char> writeBuffer;
															#line 268 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	std::vector<unsigned char> readBuffer;
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	Reference<IAsyncFile> file;
															#line 276 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int bytesWritten;
															#line 277 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int chunkSize;
															#line 284 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	int bytesRead;
															#line 2030 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase264()
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
class FlowTestCase264Actor final : public Actor<Void>, public ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >, public ActorCallback< FlowTestCase264Actor, 1, Void >, public ActorCallback< FlowTestCase264Actor, 2, Void >, public ActorCallback< FlowTestCase264Actor, 3, int >, public FastAllocated<FlowTestCase264Actor>, public FlowTestCase264ActorState<FlowTestCase264Actor> {
															#line 2035 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase264Actor>::operator new;
	using FastAllocated<FlowTestCase264Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< FlowTestCase264Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase264Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase264Actor, 3, int >;
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	FlowTestCase264Actor(UnitTestParameters const& params) 
															#line 2049 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase264ActorState<FlowTestCase264Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase264", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase264");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase264", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase264Actor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase264Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase264Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase264Actor, 3, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
static Future<Void> flowTestCase264( UnitTestParameters const& params ) {
															#line 264 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
	return Future<Void>(new FlowTestCase264Actor(params));
															#line 2080 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase264, "fdbrpc/AsyncFileEncrypted")

#line 294 "/usr/src/libfdb_c/fdbrpc/AsyncFileEncrypted.actor.cpp"
