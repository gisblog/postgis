
#include <postgres.h>
#include <liblwgeom.h>
#include <math.h>

#include <access/spgist.h>
#include <access/stratnum.h>
#include <catalog/namespace.h>
#include <catalog/pg_type.h>
#include <utils/builtins.h>
#include <utils/geo_decls.h>

/*****************************************************************************
 * Operator strategy numbers used in the GiST and SP-GiST box3d opclasses
 *****************************************************************************/

#define SPGLeftStrategyNumber            1   /* for << */
#define SPGOverLeftStrategyNumber        2   /* for &< */
#define SPGOverlapStrategyNumber         3   /* for && */
#define SPGOverRightStrategyNumber       4   /* for &> */
#define SPGRightStrategyNumber           5   /* for >> */
#define SPGSameStrategyNumber            6   /* for ~= */
#define SPGContainsStrategyNumber        7   /* for @> */
#define SPGContainedByStrategyNumber     8   /* for <@ */
#define SPGOverBelowStrategyNumber       9   /* for &<| */
#define SPGBelowStrategyNumber           10  /* for <<| */
#define SPGAboveStrategyNumber           11  /* for |>> */
#define SPGOverAboveStrategyNumber       12  /* for |&> */
#define SPGOldContainsStrategyNumber     13  /* for old spelling of @> */
#define SPGOldContainedByStrategyNumber  14  /* for old spelling of <@ */
#define SPGKNNSearchStrategyNumber       15  /* for <-> (distance) */
#define SPGContainsElemStrategyNumber    16  /* for range types @> elem */
#define SPGAdjacentStrategyNumber        17  /* for -|- */
#define SPGEqualStrategyNumber           18  /* for = */
#define SPGNotEqualStrategyNumber        19  /* for != */
#define SPGLessStrategyNumber            20  /* for < */
#define SPGLessEqualStrategyNumber       21  /* for <= */
#define SPGGreaterStrategyNumber         22  /* for > */
#define SPGGreaterEqualStrategyNumber    23  /* for >= */
#define SPGSubStrategyNumber             24  /* for inet >> */
#define SPGSubEqualStrategyNumber        25  /* for inet <<= */
#define SPGSuperStrategyNumber           26  /* for inet << */
#define SPGSuperEqualStrategyNumber      27  /* for inet >>= */
#define SPGOverFrontStrategyNumber		28		/* for &</ */
#define SPGFrontStrategyNumber			29		/* for <</ */
#define SPGBackStrategyNumber			30		/* for />> */
#define SPGOverBackStrategyNumber		31		/* for /&> */

/*****************************************************************************
 * BOX3D operators
 *****************************************************************************/


bool BOX3D_contains_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_contained_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overlaps_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_same_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_left_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overleft_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_right_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overright_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_below_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overbelow_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_above_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overabove_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_front_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overfront_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_back_internal(BOX3D *box1, BOX3D *box2);
bool BOX3D_overback_internal(BOX3D *box1, BOX3D *box2);
double BOX3D_distance_internal(BOX3D *box1, BOX3D *box2);
