//
//
//
//
//
//




#include <stdio.h>
#include <tgmath.h>
#include <string.h>
#include <float.h>          // need to use FLT_EPSILON for comparisons
#include <assert.h>         // for static_assert of padding inside the struct

#include "vector3.h"



static Vector3 test_vec3_alignment;
static_assert( sizeof(test_vec3_alignment) == sizeof(test_vec3_alignment.v),
               "Error: padding detected. Vector3 can not be represented correctly!  Going nowhere without my Vector3!\n" );


static int i = 0;           // used for counting in loops


//===============================================================================================================
//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_from_zeroes(void)
{
    Vector3 r = {0.0, 0.0, 0.0};
    return r;
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_from_values(double x, double y, double z)
{
    Vector3 r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_from_points(Vector3 from, Vector3 to)
{
    Vector3 vec = {to.x - from.x,
                   to.y - from.y,
                   to.z - from.z};

    return vec;
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_from_array(double *buffer)
{
    Vector3 r;
    memcpy( &r.v, buffer, sizeof(double) * 3 );
    return r;
}




//===============================================================================================================
//---------------------------------------------------------------------------------------------------------------
double vec3_len_squared(Vector3 vec)
{
    double len_squared = 0.0;

    for (i = 0; i < 3; i++) {
        len_squared += (vec.v[i] * vec.v[i]);
    }

    return len_squared;
}

//---------------------------------------------------------------------------------------------------------------
double vec3_len(Vector3 vec)
{
    return sqrt( vec3_len_squared(vec) );
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_norm(Vector3 vec)
{
    double len = vec3_len(vec);
    return vec3_scalar_div( vec, len );
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_scalar_div( Vector3 vec, double scalar )
{
    return vec3_scalar_mul( vec, 1.0/scalar );
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_scalar_mul( Vector3 vec, double scalar )
{
    Vector3 r;
    for (i = 0; i < 3; i++) {
        r.v[i] = (vec.v[i] * scalar);
    }

    return r;
}



//===============================================================================================================
//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_cross(Vector3 a, Vector3 b)
{

    Vector3 r = {a.y*b.z - a.z*b.y,
                 a.z*b.x - a.x*b.z,
                 a.x*b.y - a.y*b.x };
    return r;
}

//---------------------------------------------------------------------------------------------------------------
double vec3_dot(Vector3 a, Vector3 b)
{
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_add(Vector3 a, Vector3 b)
{

    Vector3 r = {a.x + b.x,
                 a.y + b.y,
                 a.z + b.z };
    return r;
}

//------------------------------------------------------------------------------------------------------------------------------------------
bool vec3_equal(Vector3 a, Vector3 b)
{
    for(i = 0; i < 3; i++) {
        if ( fabs(a.v[i] - b.v[i]) > FLT_EPSILON)
            return false;
    }

    return true;
}


//---------------------------------------------------------------------------------------------------------------
Vector3 vec3_project_plane(Vector3 vec, Vector3 normal)
{
    // @ret: result of projecting vector [ vec ] on plane defined by normal [ normal ]
    //
    // @param: [ normal ] should be of UNIT length
    // @param: [ vec ] vector that you want to project

    double dp = vec3_dot(vec, normal);
    Vector3 delta = vec3_scalar_mul(normal, -dp);
    return vec3_add(vec, delta);
}







//==========================================================================================================================================
// Unit testing facilities
#ifdef VECTOR3_UNITTEST

// You only need to include <glib-2.0/glib.h> for this to function.
// BUT when writing unit tests you might need to explicitly include <glib/gtestutils.h>
// if you want an IDE like QT Creator to do name highlighting and auto completion

#include <glib-2.0/glib.h>      // testing facilities
#include <glib/gtestutils.h>



Vector3 testvec = {-2233323.0,              // random large negative number
                    500.000000001,            // random number with small positive number
                    -0.000000089012345,       // random small negative number
                    };


// Functions to create a vector object
//---------------------------------------------------------------------------------------------------------------
void test_vec3_from_zeroes(void)
{
    Vector3 math = {0.0, 0.0, 0.0};
    Vector3 func = vec3_from_zeroes();

    g_assert_true(  vec3_equal(math, func)  );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_from_values(void)
{
    Vector3 func = vec3_from_values(testvec.x, testvec.y, testvec.z);
    g_assert_true(  vec3_equal(testvec, func)  );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_from_points(void)
{
    Vector3 math = {123223000.67566, 24.000000043233999,  -143.02342291326769};


    Vector3 a = {-123223343.0,   -0.000000043234, 143.023423};
    Vector3 b = {-342.32434,       24.0,            0.0000000867323};
    Vector3 func = vec3_from_points(a, b);

    g_assert_true(  vec3_equal(math, func)  );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_from_array(void)
{

    double a[5] = { 0.0,
                    -2233323.0,              // random large negative number
                    500.000000001,            // random number with small positive number
                    -0.000000089012345,       // random small negative number
                    0.0
                  };

    Vector3 func = vec3_from_array( &a[1] );

    g_assert_true(  vec3_equal(testvec, func)  );
}


// Functions operating on one vector
//---------------------------------------------------------------------------------------------------------------
void test_vec3_len_squared(void)
{
    double math = 4987731872329.0;
    double func = vec3_len_squared( testvec );
    g_assert_cmpfloat( math, ==, func );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_len(void)
{
    double math = 2233323.0559704076;
    double func = vec3_len( testvec );
    g_assert_cmpfloat( math, ==, func );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_norm(void)
{
    Vector3 math = {-0.99999997493850812, 0.00022388162727480712, -3.9856457292213366e-14};
    Vector3 func = vec3_norm( testvec );

    g_assert_true(  vec3_equal(math, func)  );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_scalar_div(void)
{
    Vector3 math = {691431.26934984524, -154.79876161021673, 2.7558001547987616e-08};
    Vector3 func = vec3_scalar_div( testvec, -3.23 );

    g_assert_true(  vec3_equal(math, func)  );

}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_scalar_mul(void)
{
    Vector3 math = {7213633.29, -1615.0, 2.8750987435e-07};
    Vector3 func = vec3_scalar_mul( testvec, -3.23 );

    g_assert_true(  vec3_equal(math, func)  );
}



// Functions involving different types of input
//---------------------------------------------------------------------------------------------------------------
void test_vec3_cross(void)
{
    Vector3 math = { 4194.03880448, -78.747632, -6000.00000032 };

    Vector3 a = {0.0, 75.000000004, -0.9843454};
    Vector3 b = {80.0, -6.056, 56.0};
    Vector3 func = vec3_cross(a, b);

    g_assert_true(  vec3_equal(math, func)  );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_dot(void)
{
    double math = 4987731872329.0;
    double func = vec3_dot(testvec, testvec);
    g_assert_cmpfloat( math, ==, func );
}

//---------------------------------------------------------------------------------------------------------------
void test_vec3_add(void)
{
    Vector3 math = { -4466646.0, 1000.0, -1.7802469e-07 };
    Vector3 func = vec3_add(testvec, testvec);
    g_assert_true(  vec3_equal(math, func)  );
}

//------------------------------------------------------------------------------------------------------------------------------------------
void test_vec3_equal(void)
{
    Vector3 v1 = {-2233323.0,              // random large negative number
                 500.000000001,            // random number with small positive number
                 -0.000000089012345,       // random small negative number
                 };


    // set bogus initial values
    Vector3 v2 = { -5676344.32425451, 2342342352344.2343, 14.3456223423532};

    // override with values same as vector v1
    v2.x = -2233323.0;
    v2.y = 500.000000001;
    v2.z = -0.000000089012345;

    g_assert_true(  vec3_equal(v1, v2)  );
}


//---------------------------------------------------------------------------------------------------------------
void test_vec3_project_plane(void)
{

    Vector3 math = {-1761798.2955067297, 118390.37203386797, -903755.68361219054};

    Vector3 normal = {12.0, 3.00023403, -23.0};
    Vector3 func = vec3_project_plane( testvec, vec3_norm(normal) );

    g_assert_true(  vec3_equal(math, func)  );
}










void setuptests(void)
{
    // first should test the equality function
    g_test_add_func("/set_quat/test_equal", test_vec3_equal);

    // Creation functions
    g_test_add_func("/set_vec3/test_vec3_from_zeroes", test_vec3_from_zeroes);
    g_test_add_func("/set_vec3/test_vec3_from_values", test_vec3_from_values);
    g_test_add_func("/set_vec3/test_vec3_from_points", test_vec3_from_points);
    g_test_add_func("/set_vec3/test_vec3_from_array", test_vec3_from_array);

    // Functions that operate on one vector
    g_test_add_func("/set_vec3/test_vec3_len_squared", test_vec3_len_squared);
    g_test_add_func("/set_vec3/test_vec3_len", test_vec3_len);
    g_test_add_func("/set_vec3/test_vec3_norm", test_vec3_norm);
    g_test_add_func("/set_vec3/test_vec3_scalar_div", test_vec3_scalar_div);
    g_test_add_func("/set_vec3/test_vec3_scalar_mul", test_vec3_scalar_mul);

    // Functions taking more than one vector (various inputs)
    g_test_add_func("/set_vec3/test_vec3_cross", test_vec3_cross);
    g_test_add_func("/set_vec3/test_vec3_dot", test_vec3_dot);
    g_test_add_func("/set_vec3/test_vec3_add", test_vec3_add);
    g_test_add_func("/set_vec3/test_vec3_equal", test_vec3_equal);
    g_test_add_func("/set_vec3/test_vec3_project_plane", test_vec3_project_plane);
}


int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_set_nonfatal_assertions();
    setuptests();
    return g_test_run();
}

#endif          // VECTOR3_UNITTEST




