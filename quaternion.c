#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tgmath.h>
#include <float.h>              // for FLT_EPSILON in comparison precision
#include <assert.h>             // for static_assert of padding inside the struct
#include <string.h>

#include "quaternion.h"
#include "vector3.h"


#ifndef M_PI
#define M_PI           3.14159265358979323846264338327
#endif


static Quaternion test_quat_alignment;
static_assert( sizeof(test_quat_alignment) == sizeof(test_quat_alignment.q),
               "Error: padding detected. Quaternion can not be represented correctly! Going nowhere without my Quaternion!\n");




static int i = 0;       // will be used in all loops

//==========================================================================================================================================
//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_from_identity(void)
{
    Quaternion q = {1.0, 0.0, 0.0, 0.0};
    return q;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_from_values(double w, double x, double y, double z)
{
    Quaternion q;
    q.w = w;
    q.x = x;
    q.y = y;
    q.z = z;
    return q;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_from_angle_axis(double angle, Vector3 axis)
{
    Quaternion q;
    double s = sin(angle /= 2.0) / vec3_len( axis );

    q.w = cos(angle);
    for (i = 0; i < 3; i++) {
        q.q[ i+1 ] = s * axis.v[i];
    }

    return q;
}


//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_from_vec3(Vector3 a, Vector3 b)
{
    Quaternion r = {0};
    Vector3 axis = vec3_cross(a, b);
    double dp = 0.0;

    if (vec3_equal( axis, vec3_from_zeroes() )) {
        return quat_from_identity();
    }

    dp = vec3_dot(a, b);
    r.w = sqrt(vec3_len_squared(a) * vec3_len_squared(b)) + dp;

    for (i = 0; i < 3; i++) {
        r.q[i + 1] = axis.v[i];
    }

    return quat_norm(r);
}


//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_from_euler_angles(double anglex, double angley, double anglez)
{
    Quaternion r;

    double cx = cos(anglex /= 2.0), sx = sin( anglex );
    double cy = cos(angley /= 2.0), sy = sin( angley );
    double cz = cos(anglez /= 2.0), sz = sin( anglez );

    r.w = cy*cz*cx - sy*sz*sx;
    r.x = sy*sz*cx + cy*cz*sx;
    r.y = sy*cz*cx + cy*sz*sx;
    r.z = cy*sz*cx - sy*cz*sx;

    return r;
}



//==========================================================================================================================================
//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_copy(Quaternion q)
{
    Quaternion r;

    // This is more explicit than memcpy, and the compiler will optimise this to memcpy(r.q, q.q, sizeof(double)*4) anyway.
    for(i = 0; i < 4; i++) {
        r.q[i] = q.q[i];
    }

    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
double quat_len_squared(Quaternion q)
{
    double len_2 = 0.0;

    for(i = 0; i < 4; i++) {
        len_2 += (q.q[i] * q.q[i]);
    }

    return len_2;
}

//------------------------------------------------------------------------------------------------------------------------------------------
double quat_len(Quaternion q)
{
    return sqrt( quat_len_squared(q) );
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_norm(Quaternion q)
{
    Quaternion r;
    double qlen = quat_len( q );

    for (i = 0; i < 4; i++) {
        r.q[i] = q.q[i] / qlen;
    }

    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_negate(Quaternion q)
{
    Quaternion r;

    for(i = 0; i < 4; i++) {
        r.q[i] = -q.q[i];
    }
    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_conjugate(Quaternion q)
{
    Quaternion r = {0};

    r.w = q.w;
    for(i = 1; i < 4; i++) {
        r.q[i] = -q.q[i];
    }
    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_inverse(Quaternion q)
{

    double len_2 = quat_len_squared( q );
    Quaternion r = { q.w/len_2,
                     -q.x/len_2, -q.y/len_2, -q.z/len_2};
    return r;
}



//==========================================================================================================================================
//------------------------------------------------------------------------------------------------------------------------------------------
bool quat_equal(Quaternion a, Quaternion b)
{
    for(i = 0; i < 4; i++) {
        if ( fabs(a.q[i] - b.q[i]) > FLT_EPSILON)
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
double quat_dot(Quaternion a, Quaternion b)
{
    double dp = 0.0;
    for(i = 0; i < 4; i++) {
        dp += a.q[i] * b.q[i];
    }

    return dp;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Alternative quat comparison function. quat_equal is probably faster in most cases.
bool quat_matching(Quaternion a, Quaternion b)
{
    return fabs( quat_dot(a, b) ) < FLT_EPSILON;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Quaternion quat_mul(Quaternion a, Quaternion b)
{
    Quaternion r;

    r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    r.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    r.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    r.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;

    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
Vector3 quat_rotate_vec3(Quaternion q, Vector3 v)
{
    Vector3 r;

    double vx = v.x, vy = v.y, vz = v.z;
    double w = q.w, x = q.x, y = q.y, z = q.z;

    double ww = w*w;
    double xx = x*x;
    double yy = y*y;
    double zz = z*z;
    double wx = w*x;
    double wy = w*y;
    double wz = w*z;
    double xy = x*y;
    double xz = x*z;
    double yz = y*z;

    r.x = ww*vx + xx*vx - yy*vx - zz*vx + 2*((xy-wz)*vy + (xz+wy)*vz);
    r.y = ww*vy - xx*vy + yy*vy - zz*vy + 2*((xy+wz)*vx + (yz-wx)*vz);
    r.z = ww*vz - xx*vz - yy*vz + zz*vz + 2*((xz-wy)*vx + (yz+wx)*vy);

    return r;
}


//------------------------------------------------------------------------------------------------------------------------------------------
// @param [buffer] pointer to array of at least 16 doubles in which the matrix will be stored
void quat_to_matrix44(Quaternion q, double * buffer)
{
    double xx = 2.0*q.x*q.x;
    double yy = 2.0*q.y*q.y;
    double zz = 2.0*q.z*q.z;
    double xy = 2.0*q.x*q.y;
    double zw = 2.0*q.z*q.w;
    double xz = 2.0*q.x*q.z;
    double yw = 2.0*q.y*q.w;
    double yz = 2.0*q.y*q.z;
    double xw = 2.0*q.x*q.w;

    double mat44[16] = {1.0-yy-zz, xy+zw, xz-yw, 0.0,
                        xy-zw, 1.0-xx-zz, yz+xw, 0.0,
                        xz+yw, yz-xw, 1.0-xx-yy, 0.0,
                        0.0,   0.0,   0.0,       1.0};

    memcpy( buffer, mat44, sizeof(double) * 16);
}










//==========================================================================================================================================
// Unit testing facilities
#ifdef QUATERNION_UNITTEST

// You only need to include <glib-2.0/glib.h> for this to function.
// BUT when writing unit tests you might need to explicitly include <glib/gtestutils.h>
// if you want an IDE like QT Creator to do name highlighting and auto completion

#include <glib-2.0/glib.h>      // testing facilities
#include <glib/gtestutils.h>




Quaternion testquat = {-2233323.0,              // random large negative number
                       50.000000001,            // random small positive number
                       0.123456789012345,       // assumed thresh hold (correct to at least 6 decimal places)
                       -0.0000000001            // random small precise negative number
                      };


double radian(double degrees)
{
    return degrees * (M_PI / 180.0);
}




//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_from_identity(void)
{
    Quaternion math = {1.0, 0.0, 0.0, 0.0};
    Quaternion func = quat_from_identity();

    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_from_values(void)
{
    Quaternion func = quat_from_values(testquat.w, testquat.x, testquat.y, testquat.z);
    g_assert_true(  quat_equal(testquat, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_from_angle_axis(void)
{
    Quaternion math = {
        -1.83697019872e-16,
        0.0993739857373,
        0.0,
        -0.995050154997
    };

    Vector3 axis = {0.9329932, 0.0, -9.342234};
    double angle = M_PI * -3.0;
    Quaternion func = quat_from_angle_axis( angle, axis );

    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_from_vec3(void)
{
    Quaternion math = {
        0.707049346635,
        0.00135175032552,
        0.707162919131,
        0.0
    };

    Vector3 a = {12312.0, -23.534534345, 2.0};
    Vector3 b = {0.0, 0.0, -5.0};
    Quaternion func = quat_from_vec3(a, b);

    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_from_euler_angles(void)
{
    Quaternion math = {
        0.391291076976,
        0.313887576424,
        0.863412755742,
        -0.0537056390748
    };

    double ax = radian(23.0);       // small angle with decimal (2.93429535 degrees)
    double ay = radian(-235.0);     // negative large angle (-235.0 degrees)
    double az = radian(390.0);        // angle larger than 360 degrees (390 degrees)
    Quaternion func = quat_from_euler_angles( ax, ay, az );

    g_assert_true( quat_equal(math, func) );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_copy(void)
{
    Quaternion func = quat_copy( testquat );
    g_assert_true(  quat_equal(testquat, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_len_squared(void)
{
    double math = 4987731624829.016;
    double func = quat_len_squared( testquat );
    g_assert_cmpfloat( math, ==, func );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_len(void)
{
    double math = 2233323.0005597076;
    double func = sqrt( quat_len_squared(testquat) );
    g_assert_cmpfloat( math, ==, func );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_norm(void)
{
    Quaternion math = {
        -0.999999999749,
        2.23881632834e-05,
        5.52794150158e-08,
        -4.47763265658e-17
    };

    Quaternion func = quat_norm( testquat );
    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_negate(void)
{
    Quaternion math = {
        2233323.0,
        -50.000000001,
        -0.123456789012,
        1e-10
    };

    Quaternion func = quat_negate( testquat );
    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_conjugate(void)
{
    Quaternion math = {
        -2233323.0,
        -50.000000001,
        -0.123456789012,
        1e-10
    };

    Quaternion func = quat_conjugate( testquat );
    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_inverse(void)
{
    Quaternion math = {
        -4.47763265546e-07,
        -1.00245971038e-11,
        -2.47520913912e-14,
        2.00491942073e-23
    };

    Quaternion func = quat_inverse( testquat );
    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_equal(void)
{
    Quaternion q1 = {-223332324322.0,       // random very large negative number
                     0.00000000001,           // random very small positive number
                     0.123456789012345,     // assumed thresh hold (correct to at least 6 decimal places)
                     -0.00000000001          // random very small precise negative number
                    };

    // set bogus initial values
    Quaternion q2 = {5.345534534534, 23324235435.0, -3443432.343, 0.0};

    // override with values same as q1
    q2.w = -223332324322.0;       // random very large negative number
    q2.x = 0.00000000001;           // random very small positive number
    q2.y = 0.123456789012345;     // check thresh hold (correct to at least 6 decimal places)
    q2.z = -0.00000000001;          // random very small precise negative number

    g_assert_true(  quat_equal(q1, q2)  );
}


// Functions involving different types of input
//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_dot(void)
{
    double math = 4987731624829.016;
    double func = quat_dot(testquat, testquat);

    g_assert_cmpfloat( math, ==, func );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_matching(void)
{
    g_assert_true( true );     // don't use quat_matching its deprecated.
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_mul(void)
{
    Quaternion math = {
        4.98773161983e+12,
        -223332300.004,
        -551437.772815,
        0.0004466646
    };

    Quaternion func = quat_mul(testquat, testquat);
    g_assert_true(  quat_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_quat_rotate_vec3(void)
{
    Vector3 math = {
        -43.3233235379,
        1.00143284134,
        31.9999504018
    };

    Quaternion tq = quat_norm( testquat );
    Vector3 v = {-43.32332, 1.0, 32.0};
    Vector3 func = quat_rotate_vec3( tq, v );

    for (i = 0; i < 16; i++) {
        g_assert_cmpfloat( math.v[i], ==, func.v[i] );
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
// @param [buffer] pointer to array of at least 16 doubles in which the matrix will be stored
void test_quat_to_matrix44(void)
{
    double math[16] = {
        1, 0,       0,          0,
        0, 1,       -0.000045,  0,
        0, 0.000045, 1,         0,
        0, 0,       0,          1
    };

    double func[16] = {0};
    quat_to_matrix44( testquat, func );


    for (i = 0; i < 16; i++) {
        g_assert_cmpfloat( math[i], ==, func[i] );
    }
}



void setuptests(void)
{
    // first should test the equality function
    g_test_add_func("/set_quat/test_equal", test_quat_equal);

    // Creation functions
    g_test_add_func("/set_quat/test_quat_from_identity", test_quat_from_identity);
    g_test_add_func("/set_quat/test_quat_from_values", test_quat_from_values);
    g_test_add_func("/set_quat/test_quat_from_angle_axis", test_quat_from_angle_axis);
    g_test_add_func("/set_quat/test_quat_from_vec3", test_quat_from_vec3);
    g_test_add_func("/set_quat/test_quat_from_euler_angles", test_quat_from_euler_angles);

    // Unary quaternion operations
    g_test_add_func("/set_quat/test_quat_copy", test_quat_copy);
    g_test_add_func("/set_quat/test_quat_len_squared", test_quat_len_squared);
    g_test_add_func("/set_quat/test_quat_len", test_quat_len);
    g_test_add_func("/set_quat/test_quat_norm", test_quat_norm);
    g_test_add_func("/set_quat/test_quat_negate", test_quat_negate);
    g_test_add_func("/set_quat/test_quat_conjugate", test_quat_conjugate);
    g_test_add_func("/set_quat/test_quat_inverse", test_quat_inverse);

    // Functions taking various inputs
    g_test_add_func("/set_quat/test_quat_equal", test_quat_equal);
    g_test_add_func("/set_quat/test_quat_dot", test_quat_dot);
    g_test_add_func("/set_quat/test_quat_matching", test_quat_matching);
}


int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_set_nonfatal_assertions();
    setuptests();
    return g_test_run();
}

#endif          // QUATERNION_UNITTEST


