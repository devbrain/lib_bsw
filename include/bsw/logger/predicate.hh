#ifndef BSW_LOGGER_PREDICATE_HH
#define BSW_LOGGER_PREDICATE_HH

#include <string>
#include <bsw/logger/level.hh>
#include <bsw/logger/logger_fwd.hh>

namespace bsw::logger {
	class predicate {
	 public:
		predicate () = default;
		virtual ~predicate () = default;

		[[nodiscard]] virtual bool call (record_ptr_t record) const = 0;
	};

	class not_predicate_c : public predicate {
	 public:
		explicit not_predicate_c (predicate_ptr_t operand);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const predicate_ptr_t m_operand;
	};

	class or_predicate_c : public predicate {
	 public:
		or_predicate_c (predicate_ptr_t operand1, predicate_ptr_t operand2);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const predicate_ptr_t m_operand1;
		const predicate_ptr_t m_operand2;

	};

	class and_predicate_c : public predicate {
	 public:
		and_predicate_c (predicate_ptr_t operand1, predicate_ptr_t operand2);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:

		const predicate_ptr_t m_operand1;
		const predicate_ptr_t m_operand2;

	};

	class if_predicate_c : public predicate {
	 public:
		if_predicate_c (predicate_ptr_t condition,
						predicate_ptr_t yesBranch,
						predicate_ptr_t noBranch);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:

		const predicate_ptr_t m_condition;
		const predicate_ptr_t m_yesBranch;
		const predicate_ptr_t m_noBranch;

	};

	class level_eq_c : public predicate {
	 public:
		explicit level_eq_c (level_t level);
		level_eq_c (level_t levelMin, level_t levelMax);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const level_t m_levelMin;
		const level_t m_levelMax;
		bool m_checkRange;
	};

	class line_num_eq_c : public predicate {
	 public:
		explicit line_num_eq_c (unsigned int line);
		line_num_eq_c (unsigned int lineMin, unsigned int lineMax);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const unsigned int m_lineMin;
		const unsigned int m_lineMax;
		bool m_checkRange;
	};

	class file_name_eq_c : public predicate {
	 public:
		explicit file_name_eq_c (std::string fileName);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const std::string m_fileName;

	};

	class module_name_predicate_c : public predicate {
	 public:
		explicit module_name_predicate_c (const char* moduleName);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const std::string m_moduleName;

	};

	class tag_predicate_c : public predicate {
	 public:
		explicit tag_predicate_c (const char* tag);
	 private:
		[[nodiscard]] bool call (record_ptr_t record) const override;
	 private:
		const std::string m_tag;
	};
} // ns logger




#endif
