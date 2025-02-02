// Copyright (C) 2009-2023, Panagiotis Christopoulos Charitos and contributors.
// All rights reserved.
// Code licensed under the BSD License.
// http://www.anki3d.org/LICENSE

#pragma once

#include <AnKi/Util/StdTypes.h>
#include <AnKi/Util/Enum.h>
#include <AnKi/Util/Ptr.h>
#include <AnKi/Util/MemoryPool.h>
#include <AnKi/Math.h>

#if ANKI_COMPILER_GCC_COMPATIBLE
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wall"
#	pragma GCC diagnostic ignored "-Wconversion"
#	pragma GCC diagnostic ignored "-Wfloat-conversion"
#	if(ANKI_COMPILER_GCC && __GNUC__ >= 9) || (ANKI_COMPILER_CLANG && __clang_major__ >= 10)
#		pragma GCC diagnostic ignored "-Wdeprecated-copy"
#	endif
#endif
#if ANKI_COMPILER_MSVC
#	pragma warning(push)
#	pragma warning(disable : 4305)
#endif
#define BT_THREADSAFE 0
#define BT_NO_PROFILE 1
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#if ANKI_COMPILER_GCC_COMPATIBLE
#	pragma GCC diagnostic pop
#endif
#if ANKI_COMPILER_MSVC
#	pragma warning(pop)
#endif

namespace anki {

#define ANKI_PHYS_LOGI(...) ANKI_LOG("PHYS", kNormal, __VA_ARGS__)
#define ANKI_PHYS_LOGE(...) ANKI_LOG("PHYS", kError, __VA_ARGS__)
#define ANKI_PHYS_LOGW(...) ANKI_LOG("PHYS", kWarning, __VA_ARGS__)
#define ANKI_PHYS_LOGF(...) ANKI_LOG("PHYS", kFatal, __VA_ARGS__)

class PhysicsMemoryPool : public HeapMemoryPool, public MakeSingleton<PhysicsMemoryPool>
{
	template<typename>
	friend class MakeSingleton;

private:
	PhysicsMemoryPool(AllocAlignedCallback allocCb, void* allocCbUserData)
		: HeapMemoryPool(allocCb, allocCbUserData, "PhysicsMemPool")
	{
	}

	~PhysicsMemoryPool() = default;
};

ANKI_DEFINE_SUBMODULE_UTIL_CONTAINERS(Physics, PhysicsMemoryPool)

// Forward
class PhysicsObject;
class PhysicsFilteredObject;
class PhysicsWorld;
class PhysicsCollisionShape;
class PhysicsBody;
class PhysicsPlayerController;
class PhysicsJoint;
class PhysicsTrigger;

/// @addtogroup physics
/// @{

/// PhysicsPtr custom deleter.
class PhysicsPtrDeleter
{
public:
	void operator()(PhysicsObject* ptr);
};

/// Smart pointer for physics objects.
template<typename T>
using PhysicsPtr = IntrusivePtr<T, PhysicsPtrDeleter>;

using PhysicsObjectPtr = PhysicsPtr<PhysicsObject>;
using PhysicsFilteredObjectPtr = PhysicsPtr<PhysicsFilteredObject>;
using PhysicsCollisionShapePtr = PhysicsPtr<PhysicsCollisionShape>;
using PhysicsBodyPtr = PhysicsPtr<PhysicsBody>;
using PhysicsPlayerControllerPtr = PhysicsPtr<PhysicsPlayerController>;
using PhysicsJointPtr = PhysicsPtr<PhysicsJoint>;
using PhysicsTriggerPtr = PhysicsPtr<PhysicsTrigger>;

/// Material types.
enum class PhysicsMaterialBit : U64
{
	kNone = 0,
	kStaticGeometry = 1 << 0,
	kDynamicGeometry = 1 << 1,
	kTrigger = 1 << 2,
	kPlayer = 1 << 3,
	kParticle = 1 << 4,

	kAll = kMaxU64
};
ANKI_ENUM_ALLOW_NUMERIC_OPERATIONS(PhysicsMaterialBit)

[[nodiscard]] inline Vec3 toAnki(const btVector3& v)
{
	return Vec3(v.getX(), v.getY(), v.getZ());
}

[[nodiscard]] inline btVector3 toBt(const Vec3& v)
{
	return btVector3(v.x(), v.y(), v.z());
}

[[nodiscard]] inline btTransform toBt(const Transform& a)
{
	Mat4 mat(a);
	mat.transpose();
	btTransform out;
	out.setFromOpenGLMatrix(&mat(0, 0));
	return out;
}

[[nodiscard]] inline Mat3x4 toAnki(const btMatrix3x3& m)
{
	Mat3x4 m3;
	m3.setRows(Vec4(toAnki(m[0]), 0.0f), Vec4(toAnki(m[1]), 0.0f), Vec4(toAnki(m[2]), 0.0f));
	return m3;
}

[[nodiscard]] inline Transform toAnki(const btTransform& t)
{
	Transform out;
	out.setRotation(toAnki(t.getBasis()));
	out.setOrigin(Vec4(toAnki(t.getOrigin()), 0.0f));
	out.setScale(1.0f);
	return out;
}
/// @}

} // end namespace anki
