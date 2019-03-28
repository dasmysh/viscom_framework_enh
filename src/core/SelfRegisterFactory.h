/**
 * @file   SelfRegisterFactory.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2019.01.21
 *
 * @brief  Definition of a factory class that allows self registration of types.
   Inspired by https://www.bfilipek.com/2018/02/factory-selfregister.html
 */

#pragma once

#include <memory>
#include <functional>
#include <algorithm>
#include <execution>
#include <tuple>

namespace viscom {
    class ApplicationNodeImplementation;
}


namespace mysh::core {

    template<class T>
    class SelfRegisterFactory
    {
    public:
        using TCreateMethod = std::function<std::unique_ptr<T>()>;
        using TMethodList = std::vector<std::tuple<int, std::string, TCreateMethod>>;
        using TAppGetter = std::function<viscom::ApplicationNodeImplementation*()>;

    public:
        SelfRegisterFactory() = delete;
        static void Initialize(const std::function<viscom::ApplicationNodeImplementation*()>& appNodeGetter) { s_appNodeGetter() = appNodeGetter; }
        static viscom::ApplicationNodeImplementation* GetAppNode() { return s_appNodeGetter()(); }

        static bool Register(const std::string_view name, TCreateMethod funcCreate, int prio = 0)
        {
            bool inserted = false;
#pragma warning(push)
#pragma warning(disable : 4834)
            if (auto it = std::find_if(std::execution::seq, s_methods().begin(), s_methods().end(),
                    [&](const auto& e) { return std::get<1>(e) == name; });
                    it == s_methods().end()) {
                s_methods().emplace_back(prio, name, funcCreate);
                inserted = true;
            }
#pragma warning(pop)
            std::sort(s_methods().begin(), s_methods().end(), [](const auto& a, const auto& b) {
                if (std::get<0>(a) == std::get<0>(b)) return std::get<1>(a) < std::get<1>(b);
                return std::get<0>(a) < std::get<0>(b);
            });
            return inserted;
        }

        static std::size_t GetNumRegistered() { return s_methods().size(); }

        static std::unique_ptr<T> Create(const std::string_view& name)
        {
            if (auto it = std::find_if(std::execution::seq, s_methods().begin(), s_methods().end(),
                    [&](const auto& e) { return std::get<1>(e) == name; });
                    it != s_methods().end()) {
                return std::get<2>(*it)();
            }
            return nullptr;
        }

        static std::unique_ptr<T> Create(std::size_t index)
        {
            if (index < s_methods().size()) return std::get<2>(s_methods()[index])();
            return nullptr;
        }

        static TMethodList& s_methods() {
            static TMethodList s_methods_;
            return s_methods_;
        }

        static TAppGetter& s_appNodeGetter() {
            static TAppGetter s_appNodeGetter_;
            return s_appNodeGetter_;
        }
    };
}


#define REGISTER_CLASS_PRIO_APP(b, c, p) \
bool c::s_registered = mysh::core::SelfRegisterFactory<b>::Register( \
    c::GetName(), []() { return std::make_unique<c>(mysh::core::SelfRegisterFactory<b>::GetAppNode()); }, p)
#define REGISTER_CLASS_PRIO(b, c, p) \
bool c::s_registered = mysh::core::SelfRegisterFactory<b>::Register( \
    c::GetName(), []() { return std::make_unique<c>(); }, p)

#define REGISTER_CLASS_APP(b, c) REGISTER_CLASS_PRIO_APP(b, c, 0)
#define REGISTER_CLASS(b, c) REGISTER_CLASS_PRIO(b, c, 0)
