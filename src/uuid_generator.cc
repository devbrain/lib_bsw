//
// Created by igor on 3/2/24.
//

#include <bsw/digest/md5.hh>
#include <bsw/digest/sha1.hh>
#include <bsw/exception.hh>
#include <bsw/io/random_stream.hh>
#include <bsw/singleton.hh>
#include <bsw/uuid_generator.hh>

namespace bsw {
	uuid_generator::uuid_generator ()
		: m_ticks (0), m_have_node (false) {
	}

	uuid_generator::~uuid_generator () = default;

	uuid uuid_generator::create () {
		std::lock_guard<std::mutex> lock (m_mutex);

		if (!m_have_node) {
			Environment::nodeId (m_node);
			m_have_node = true;
		}
		auto tv = std::chrono::steady_clock::now ().time_since_epoch ().count ();
		auto timeLow = uint32_t (tv & 0xFFFFFFFF);
		auto timeMid = uint16_t ((tv >> 32) & 0xFFFF);
		uint16_t timeHiAndVersion = uint16_t ((tv >> 48) & 0x0FFF) + (uuid::UUID_TIME_BASED << 12);
		uint16_t clockSeq = (uint16_t (m_random.next () >> 4) & 0x3FFF) | 0x8000;
		return {timeLow, timeMid, timeHiAndVersion, clockSeq, m_node};
	}

	uuid uuid_generator::create_from_name (const uuid& nsid, const std::string& name) {
		md5_engine md5;
		return create_from_name (nsid, name, md5);
	}

	uuid uuid_generator::create_from_name (const uuid& nsid, const std::string& name, digest_engine& de) {
		uuid::version_t version = uuid::UUID_NAME_BASED;
		if (dynamic_cast<sha1_engine*>(&de)) { version = uuid::UUID_NAME_BASED_SHA1; }
		return create_from_name (nsid, name, de, version);
	}

	uuid
	uuid_generator::create_from_name (const uuid& nsid, const std::string& name, digest_engine& de, uuid::version_t version) {
		ENFORCE (de.digest_length () >= 16);

		uuid netNsid = nsid;
		netNsid.to_network ();
		de.reset ();
		de.update (&netNsid._timeLow, sizeof (netNsid._timeLow));
		de.update (&netNsid._timeMid, sizeof (netNsid._timeMid));
		de.update (&netNsid._timeHiAndVersion, sizeof (netNsid._timeHiAndVersion));
		de.update (&netNsid._clockSeq, sizeof (netNsid._clockSeq));
		de.update (&netNsid._node[0], sizeof (netNsid._node));
		de.update (name);
		char buffer[16];
		const auto& d = de.digest ();
		for (int i = 0; i < 16; ++i) {
			buffer[i] = d[i];
		}
		return {buffer, version};
	}

	uuid uuid_generator::create_random () {
		char buffer[16];
		io::random_input_stream ris;
		ris.read (buffer, sizeof (buffer));
		return {buffer, uuid::UUID_RANDOM};
	}

	long uuid_generator::time_stamp () {
		auto now = std::chrono::steady_clock::now ();
		for (;;) {
			if (now != m_last_time) {
				m_last_time = now;
				m_ticks = 0;
				break;
			}
			if (m_ticks < 100) {
				++m_ticks;
				break;
			}
			now = std::chrono::steady_clock::now ();
		}
		auto tv = now.time_since_epoch ().count ();
#include <bsw/warn/push.hh>
#include <bsw/warn/possible_loss_of_data_op>
		return tv + m_ticks;
#include <bsw/warn/pop.hh>
	}

	uuid uuid_generator::create_one () {
		try {
			return create ();
		}
		catch (...) {
			return create_random ();
		}
	}

	void uuid_generator::seed () {
		std::lock_guard<std::mutex> lock (m_mutex);

		m_random.seed ();
	}

	void uuid_generator::seed (uint32_t n) {
		std::lock_guard<std::mutex> lock (m_mutex);

		m_random.seed (n);
	}

	using default_uuid_generator = singleton<uuid_generator>;

	uuid_generator& uuid_generator::default_generator () {
		return default_uuid_generator::instance ();
	}

}