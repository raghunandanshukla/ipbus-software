/**
	@file
	@author Andrew W. Rose
	@author Marc Magrans De Abril
	@date 2012
*/

#ifndef _uhal_ClientInterface_hpp_
#define _uhal_ClientInterface_hpp_

#include "uhal/exception.hpp"
#include "uhal/definitions.hpp"
#include "uhal/ValMem.hpp"

#include "uhal/log.hpp"

#include "uhal/ProtocolInterfaces.hpp"

#include "BoostSpiritGrammars/URLGrammar.hpp"

#include <vector>
#include <deque>
#include <iostream>

#include <map>

#include <uhal/performance.hpp>


namespace uhal
{
	// //! Exception class to handle the case where an Atomic Transaction was requested but could not be performed. Uses the base uhal::exception implementation of what()
	// class AtomicTransactionSize: public uhal::exception {};
	//! Exception class to handle the case where pinging of a client failed. Uses the base uhal::exception implementation of what()
	class PingFailed: public uhal::exception {};

	//! An abstract base class for defining the interface to the various IPbus clients as well as providing the generalized packing funcationality
	class ClientInterface
	{
		public:
			/**
				Constructor
				@param aId the uinique identifier that the client will be given.
				@param aUri a struct containing the full URI of the target.
			*/
			ClientInterface ( const std::string& aId, const URI& aUri );

			/**
				Destructor
			*/
			virtual ~ClientInterface();

			/**
				Return the identifier of the target for this client
				@return the identifier of the target for this client
			*/
			const std::string& id();

			/**
				Ping the target for this client
			*/
			void ping();

			/**
				Return the url of the target for this client
				@return the url of the target for this client
			*/
			std::string url();

			
			/*
				Return a description of the behaviour this client
				@return a description of the behaviour this client
			*/
			//static std::string description();
			
			/**
				Write a single, unmasked word to a register
				@param aAddr the address of the register to write
				@param aValue the value to write to the register
			*/
			virtual void write ( const uint32_t& aAddr, const uint32_t& aValue );

			/**
				Write a single, masked word to a register
				@param aAddr the address of the register to write
				@param aValue the value to write to the register
				@param aMask the mask to apply to the value
			*/
			virtual void write ( const uint32_t& aAddr, const uint32_t& aValue, const uint32_t& aMask );

			/**
				Write a block of data to a block of registers or a block-write port
				@param aAddr the address of the register to write
				@param aValues the values to write to the registers or a block-write port
				@param aMode whether we are writing to a block of registers (INCREMENTAL) or a block-write port (NON_INCREMENTAL)
			*/
			virtual void writeBlock ( const uint32_t& aAddr, const std::vector< uint32_t >& aValues, const defs::BlockReadWriteMode& aMode=defs::INCREMENTAL );

			/**
				Read a single, unmasked, unsigned word
				@param aAddr the address of the register to read
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< uint32_t > read ( const uint32_t& aAddr );

			/**
				Read a single, masked, unsigned word
				@param aAddr the address of the register to read
				@param aMask the mask to apply to the value after reading
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< uint32_t > read ( const uint32_t& aAddr, const uint32_t& aMask );

			/**
				Read a block of unsigned data from a block of registers or a block-read port
				@param aAddr the lowest address in the block of registers or the address of the block-read port
				@param aSize the number of words to read
				@param aMode whether we are reading from a block of registers (INCREMENTAL) or a block-read port (NON_INCREMENTAL)
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValVector< uint32_t > readBlock ( const uint32_t& aAddr, const uint32_t& aSize, const defs::BlockReadWriteMode& aMode=defs::INCREMENTAL );

			/**
				Read a single, unmasked word and interpret it as being signed
				@param aAddr the address of the register to read
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< int32_t > readSigned ( const uint32_t& aAddr );

			/**
				Read a single word, mask it and interpret it as being signed
				@param aAddr the address of the register to read
				@param aMask the mask to apply to the value after reading
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< int32_t > readSigned ( const uint32_t& aAddr, const uint32_t& aMask );

			/**
				Read a block of data from a block of registers or a block-read port and interpret it as being signed data
				@param aAddr the lowest address in the block of registers or the address of the block-read port
				@param aSize the number of words to read
				@param aMode whether we are reading from a block of registers (INCREMENTAL) or a block-read port (NON_INCREMENTAL)
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValVector< int32_t > readBlockSigned ( const uint32_t& aAddr, const uint32_t& aSize, const defs::BlockReadWriteMode& aMode=defs::INCREMENTAL );

			/**
				Retrieve the reserved address information from the target
				@return a Validated Memory which wraps the location to which the reserved address information will be written
			*/
			virtual ValVector< uint32_t > readReservedAddressInfo ();

			/**
				Read the value of a register, apply the AND-term, apply the OR-term, set the register to this new value and return a copy of the new value to the user
				@param aAddr the address of the register to read, modify, write
				@param aANDterm the AND-term to apply to existing value in the target register
				@param aORterm the OR-term to apply to existing value in the target register
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< uint32_t > rmw_bits ( const uint32_t& aAddr , const uint32_t& aANDterm , const uint32_t& aORterm );

			/**
				Read the value of a register, add the addend, set the register to this new value and return a copy of the new value to the user
				@param aAddr the address of the register to read, modify, write
				@param aAddend the addend to add to the existing value in the target register
				@return a Validated Memory which wraps the location to which the reply data is to be written
			*/
			virtual ValWord< int32_t > rmw_sum ( const uint32_t& aAddr , const int32_t& aAddend );


			/**
				Virtual method to add an IPbusPacketInfo to the queue of IPbusPacketInfo's
				@param aIPbusPacketInfo an IPbusPacketInfo to add to the queue of IPbusPacketInfo's
			*/
			virtual void pack ( IPbusPacketInfo& aIPbusPacketInfo ) ;

			/**
				Method to dispatch all IPbus packets which are in the queue of IPbusPacketInfo's
			*/
			void dispatch ();

		private:

			/**
				Pure virtual function to get the packing protocol used by the concrete implementation
				@return a reference to the packing protocol used by the concrete implementation
			*/
			virtual PackingProtocol& getPackingProtocol() = 0;

			/**
				Pure virtual function to get the transport protocol used by the concrete implementation
				@return a reference to the transport protocol used by the concrete implementation
			*/
			virtual TransportProtocol& getTransportProtocol() = 0;

		private:

			//! A deque of validated memory - needed since the reply memory must be valid when the dispatch is called even if the user discards their copy of the validated memory
			std::deque< ValWord< uint32_t > > mUnsignedReplyWords;
			//! A deque of validated memory - needed since the reply memory must be valid when the dispatch is called even if the user discards their copy of the validated memory
			std::deque< ValWord< int32_t > > mSignedReplyWords;
			//! A deque of validated memory - needed since the reply memory must be valid when the dispatch is called even if the user discards their copy of the validated memory
			std::deque< ValVector< uint32_t > > mUnsignedReplyVectors;
			//! A deque of validated memory - needed since the reply memory must be valid when the dispatch is called even if the user discards their copy of the validated memory
			std::deque< ValVector< int32_t > > mSignedReplyVectors;


		protected:
			//! the identifier of the target for this client
			std::string mId;
			//! a struct containing the full URI of the target for this client
			URI mUri;

	};



}

#endif

