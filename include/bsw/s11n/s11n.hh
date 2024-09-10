//
// Created by igor on 8/31/24.
//

#ifndef BSW_S11N_HH
#define BSW_S11N_HH

#include <tuple>
#include <array>
#include <string_view>
#include <vector>
#include <bsw/macros.hh>
#include <bsw/s11n/detail/archive.hh>
#include <bsw/mp/type_name/type_name.hpp>

#define PRINT_SERIALIZATION_TUPLE_(i, x) std::make_tuple<std::size_t, std::string_view>(i , PPCAT(STRINGIZE(x), sv)),
#define SERIALIZATION_SCHEMA(...)                                                                                           \
    static constexpr std::array<std::tuple<std::size_t, std::string_view>, PP_NARG(__VA_ARGS__)> serialization_schema() {   \
        using namespace std::literals;                                                                                      \
        return {FOR_EACH_COUNT(PRINT_SERIALIZATION_TUPLE_, __VA_ARGS__)};	                                                \
    }

namespace bsw::s11n {
    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    void serialize_to_yaml(const T& obj, std::ostream& os) {
        auto ar = detail::serialization_archive::create_yaml();
        detail::serialize_object(obj, ar);
        ar.serialize(os);
    }

    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    void deserialize_from_yaml(T& obj, std::istream& is) {
        auto ar = detail::deserialization_archive::create_yaml();
        ar.open(is);
        detail::deserialize_object(obj, ar);
    }

    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    T deserialize_from_yaml(std::istream& is) {
        T obj{};
        deserialize_from_yaml(obj, is);
        return obj;
    }

    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    void serialize_to_json(const T& obj, std::ostream& os) {
        auto ar = detail::serialization_archive::create_json();
        detail::serialize_object(obj, ar);
        ar.serialize(os);
    }

    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    void deserialize_from_json(T& obj, std::istream& is) {
        auto ar = detail::deserialization_archive::create_json();
        ar.open(is);
        detail::deserialize_object(obj, ar);
    }

    template<typename T, class = std::enable_if_t <std::is_class_v <T>>>
    T deserialize_from_json(std::istream& is) {
        T obj{};
        deserialize_from_json(obj, is);
        return obj;
    }

    template<typename T>
    std::vector<unsigned char> serialize_binary(const T& obj) {
        return cista::serialize(obj);
    }

    template<typename T>
    void serialize_binary(const T& obj, std::ostream& os) {
        auto buf = serialize_binary(obj);
        os.write(buf.data(), buf.size());
    }

    template <typename T>
    void deserialize_binary(T& obj, std::vector<unsigned char>& buf) {
        obj = cista::deserialize<T>(buf);
    }

    template <typename T>
    T deserialize_binary(std::vector<unsigned char>& buf) {
        return cista::deserialize<T>(buf);
    }

    template <typename T>
    T deserialize_binary(std::istream& is) {
        auto pos = is.tellg();
        is.seekg(0, std::ios::end);
        auto end = is.tellg();
        is.seekg(pos, std::ios::beg);
        std::size_t len = end - pos;
        std::vector<unsigned char> buf(len);
        is.read(reinterpret_cast<char*>(buf.data()), static_cast<std::streamsize>(len));
        auto* obj = cista::deserialize<T>(buf);
        if (!obj) {
            RAISE_EX("Deserialization of ", type_name_v<T>, " failed");
        }
        return *obj;
    }

    template <typename T>
    void deserialize_binary(T& obj, std::istream& is) {
        obj = deserialize_binary<T>(is);
    }

}

#endif
