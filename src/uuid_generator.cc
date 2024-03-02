//
// Created by igor on 3/2/24.
//

#include <bsw/digest/md5.hh>
#include <bsw/digest/sha1.hh>
#include <bsw/exception.hh>
#include <bsw/io/random_stream.hh>
#include <bsw/uuid_generator.hh>

namespace bsw {
	UUIDGenerator::UUIDGenerator(): _ticks(0), _haveNode(false)
	{
	}


	UUIDGenerator::~UUIDGenerator()
	{
	}


	uuid UUIDGenerator::create()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (!_haveNode)
		{
			Environment::nodeId(_node);
			_haveNode = true;
		}
		Timestamp::UtcTimeVal tv = timeStamp();
		uint32_t timeLow = uint32_t(tv & 0xFFFFFFFF);
		uint16_t timeMid = uint16_t((tv >> 32) & 0xFFFF);
		uint16_t timeHiAndVersion = uint16_t((tv >> 48) & 0x0FFF) + (uuid::UUID_TIME_BASED << 12);
		uint16_t clockSeq = (uint16_t(_random.next() >> 4) & 0x3FFF) | 0x8000;
		return uuid(timeLow, timeMid, timeHiAndVersion, clockSeq, _node);
	}


	uuid UUIDGenerator::createFromName(const uuid& nsid, const std::string& name)
	{
		md5_engine md5;
		return createFromName(nsid, name, md5);
	}


	uuid UUIDGenerator::createFromName(const uuid& nsid, const std::string& name, digest_engine& de)
	{
		uuid::version_t version = uuid::UUID_NAME_BASED;
		if (dynamic_cast<sha1_engine*>(&de)) version = uuid::UUID_NAME_BASED_SHA1;
		return createFromName(nsid, name, de, version);
	}


	uuid UUIDGenerator::createFromName(const uuid& nsid, const std::string& name, digest_engine& de, uuid::version_t version)
	{
		ENFORCE (de.digest_length() >= 16);

		uuid netNsid = nsid;
		netNsid.to_network();
		de.reset();
		de.update(&netNsid._timeLow, sizeof(netNsid._timeLow));
		de.update(&netNsid._timeMid, sizeof(netNsid._timeMid));
		de.update(&netNsid._timeHiAndVersion, sizeof(netNsid._timeHiAndVersion));
		de.update(&netNsid._clockSeq, sizeof(netNsid._clockSeq));
		de.update(&netNsid._node[0], sizeof(netNsid._node));
		de.update(name);
		char buffer[16];
		const auto& d = de.digest();
		for (int i = 0; i < 16; ++i)
		{
			buffer[i] = d[i];
		}
		return uuid(buffer, version);
	}


	uuid UUIDGenerator::createRandom()
	{
		char buffer[16];
		io::random_input_stream ris;
		ris.read(buffer, sizeof(buffer));
		return uuid(buffer, uuid::UUID_RANDOM);
	}


	Timestamp::UtcTimeVal UUIDGenerator::timeStamp()
	{
		Timestamp now;
		for (;;)
		{
			if (now != _lastTime)
			{
				_lastTime = now;
				_ticks = 0;
				break;
			}
			if (_ticks < 100)
			{
				++_ticks;
				break;
			}
			now.update();
		}
		Timestamp::UtcTimeVal tv = now.utcTime();
		return tv + _ticks;
	}


	uuid UUIDGenerator::createOne()
	{
		try
		{
			return create();
		}
		catch (...)
		{
			return createRandom();
		}
	}


	void UUIDGenerator::seed()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_random.seed();
	}


	void UUIDGenerator::seed(uint32_t n)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_random.seed(n);
	}


	namespace
	{
		static SingletonHolder<UUIDGenerator> sh;
	}


	UUIDGenerator& UUIDGenerator::defaultGenerator()
	{
		return *sh.get();
	}

}