#include <Core/Math/LinearAlgebra.hpp>

namespace Ra
{
    namespace Core
    {
        template<typename Vector_>
        inline Vector_ Vector::floor ( const Vector_& v )
        {
            typedef typename Vector_::Scalar Scalar_;
            Vector_ result ( v );
            result.unaryExpr ( std::function<Scalar_ ( Scalar_ )> ( static_cast<Scalar_ ( & ) ( Scalar_ )> ( std::floor ) ) );
            return result;
        }

        template<typename Vector_>
        inline Vector_ Vector::ceil ( const Vector_& v )
        {
            typedef typename Vector_::Scalar Scalar_;
            Vector_ result ( v );
            result.unaryExpr ( std::function<Scalar_ ( Scalar_ )> ( static_cast<Scalar_ ( & ) ( Scalar_ )> ( std::ceil ) ) );
            return result;
        }

        template<typename Vector_>
        inline Vector_ Vector::clamp ( const Vector_& v, const Vector_& min, const Vector_& max )
        {
            return v.cwiseMin(max).cwiseMax(min);
        }
        
        template<typename Vector_>
        inline Vector_ Vector::clamp ( const Vector_& v, const Scalar& min, const Scalar& max )
        {
            return v.cwiseMin(max).cwiseMax(min);
        }
        
        template<typename Vector_>
        inline bool Vector::check_range ( const Vector_& v, const Scalar& min, const Scalar& max )
        {
            return Vector::clamp(v, min, max) == v;
        }

        Quaternion operator+ ( const Quaternion& q1, const Quaternion& q2 )
        {
            return Quaternion ( q1.coeffs() + q2.coeffs() );
        }

        Quaternion operator* ( const Scalar& k, const Quaternion& q )
        {
            return Quaternion ( k * q.coeffs() );
        }
        
        inline void aabb_add_point(Aabb& aabb, const Vector3& p) // TODO: remove this function and replace it in the IBL
        {
            aabb.extend(p);
        }
        
        inline Aabb Obb::to_bbox() const 
        {
            Aabb tmp;
            for(int i = 0; i < 8; ++i)
                aabb_add_point(tmp, _tr * this->corner(static_cast<Aabb::CornerType>(i)));
            return tmp;
        }
        
        inline void coordinate_system(const Vector3& origin, const Vector3& fx, Vector3& fy, Vector3& fz)
        {
            //for numerical stability, and seen that z will always be present, take the greatest component between x and y.
            if(fabsf(fx(0)) > fabsf(fx(1)) )
            {
                float inv_len = 1.f / sqrtf(fx(0) * fx(0) + fx(2) * fx(2));
                Vector3 tmp(-fx(2) * inv_len, 0.f, fx(0) * inv_len);
                fy = tmp;
            } else {
                float inv_len = 1.f / sqrtf(fx(1) * fx(1) + fx(2) * fx(2));
                Vector3 tmp(0.f, fx(2) * inv_len, -fx(1) * inv_len);
                fy = tmp;
            }
            fz = fx.cross(fy);
        }
        
        inline void Obb::add_point(const Vector3& p)
        {
            // TODO: take the transform into account, but then apply the changes in capsule_implicit.cpp
            extend(p); 
        }
        
        inline Vector2 interval_squared(const Vector2& i)
        {
            if (i(0) >= 0)
                return Vector2(i(0) * i(0), i(1) * i(1));

            if (i(1) <= 0)
                return Vector2(i(1) * i(1), i(0) * i(0));

            if (-i(0) >= i(1))
                return Vector2(0, i(0) * i(0));

            return Vector2(0, i(0) * i(1));
        }
    }
}
