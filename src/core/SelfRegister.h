/**
 * @file   SelfRegister.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2019.01.22
 *
 * @brief  Definition of macros to handle classes that need to self register at a factory.
 */

#pragma once

#define CLASS_NAME(n) \
public: \
static std::string_view GetName() { return #n; } \
private: \
static bool s_registered
