/*--------------------------------------------------------------------
 *    The MB-system:	mbsys_jstar.c	10/4/94
 *	$Id$
 *
 *    Copyright (c) 2005-2017 by
 *    David W. Caress (caress@mbari.org)
 *      Monterey Bay Aquarium Research Institute
 *      Moss Landing, CA 95039
 *    and Dale N. Chayes (dale@ldeo.columbia.edu)
 *      Lamont-Doherty Earth Observatory
 *      Palisades, NY 10964
 *
 *    See README file for copying and redistribution conditions.
 *--------------------------------------------------------------------*/
/*
 * mbsys_jstar.c contains the functions for handling the data structure
 * used by MBIO functions to store data from Edgetech
 * subbottom and sidescan sonar systems.
 * The native data format used to store Edgetech
 * data is called Jstar and is supported by the format:
 *      MBF_EDGJSTAR : MBIO ID 132
 *
 * Author:	D. W. Caress
 * Date:	May 4, 2005
 *
 */

/* standard include files */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/* mbio include files */
#include "mb_status.h"
#include "mb_format.h"
#include "mb_io.h"
#include "mb_define.h"
#include "mb_process.h"
#include "mb_aux.h"
#include "mb_segy.h"
#include "mbsys_jstar.h"

static char svn_id[] = "$Id$";

/*--------------------------------------------------------------------*/
int mbsys_jstar_alloc(int verbose, void *mbio_ptr, void **store_ptr, int *error) {
	char *function_name = "mbsys_jstar_alloc";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:   %p\n", (void *)mbio_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* allocate memory for data structure */
	status = mb_mallocd(verbose, __FILE__, __LINE__, sizeof(struct mbsys_jstar_struct), (void **)store_ptr, error);

	/* make sure trace pointers are null */
	store = (struct mbsys_jstar_struct *)mb_io_ptr->store_data;
	store->kind = MB_DATA_NONE;
	store->sbp.trace_alloc = 0;
	store->sbp.trace = NULL;
	store->ssport.trace_alloc = 0;
	store->ssport.trace = NULL;
	store->ssstbd.trace_alloc = 0;
	store->ssstbd.trace = NULL;
	store->comment.comment[0] = 0;

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)*store_ptr);
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_deall(int verbose, void *mbio_ptr, void **store_ptr, int *error) {
	char *function_name = "mbsys_jstar_deall";
	int status = MB_SUCCESS;
	struct mbsys_jstar_struct *store;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:   %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)*store_ptr);
	}

	/* deallocate memory for data structure */
	if (*store_ptr != NULL) {
		store = (struct mbsys_jstar_struct *)*store_ptr;
		if (store->sbp.trace != NULL && store->sbp.trace_alloc > 0) {
			status = mb_freed(verbose, __FILE__, __LINE__, (void **)&(store->sbp.trace), error);
		}
		store->sbp.trace_alloc = 0;
		if (store->ssport.trace != NULL && store->ssport.trace_alloc > 0) {
			status = mb_freed(verbose, __FILE__, __LINE__, (void **)&(store->ssport.trace), error);
		}
		store->ssport.trace_alloc = 0;
		if (store->ssstbd.trace != NULL && store->ssstbd.trace_alloc > 0) {
			status = mb_freed(verbose, __FILE__, __LINE__, (void **)&(store->ssstbd.trace), error);
		}
		store->ssstbd.trace_alloc = 0;
	}
	status = mb_freed(verbose, __FILE__, __LINE__, (void **)store_ptr, error);

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_dimensions(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int *nbath, int *namp, int *nss, int *error) {
	char *function_name = "mbsys_jstar_dimensions";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* get beam and pixel numbers */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);
		*nbath = 1;
		*namp = 0;
		*nss = 0;
	}
	else if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);
		*nbath = 1;
		*namp = 0;
		if ((ssport->samples + ssstbd->samples) > MBSYS_JSTAR_PIXELS_MAX)
			*nss = MBSYS_JSTAR_PIXELS_MAX;
		else
			*nss = ssport->samples + ssstbd->samples;
	}
	else {
		/* get beam and pixel numbers */
		*nbath = 0;
		*namp = 0;
		*nss = 0;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
		fprintf(stderr, "dbg2       nbath:      %d\n", *nbath);
		fprintf(stderr, "dbg2        namp:      %d\n", *namp);
		fprintf(stderr, "dbg2        nss:       %d\n", *nss);
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_pingnumber(int verbose, void *mbio_ptr, int *pingnumber, int *error) {
	char *function_name = "mbsys_jstar_pingnumber";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	int kind;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)mb_io_ptr->store_data;

	/* get data kind */
	kind = store->kind;

	/* extract data from structure */
	if (kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);
		*pingnumber = sbp->pingNum;
	}
	else if (kind == MB_DATA_DATA || kind == MB_DATA_SIDESCAN2) {
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);
		*pingnumber = ssport->pingNum;
	}
	else {
		*pingnumber = mb_io_ptr->ping_count;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       pingnumber: %d\n", *pingnumber);
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_preprocess(int verbose,     /* in: verbosity level set on command line 0..N */
                           void *mbio_ptr,  /* in: see mb_io.h:/^struct mb_io_struct/ */
                           void *store_ptr, /* in: see mbsys_reson7k.h:/^struct mbsys_reson7k_struct/ */
                           void *platform_ptr, void *preprocess_pars_ptr, int *error) {
	char *function_name = "mbsys_jstar_preprocess";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	struct mb_platform_struct *platform;
	struct mb_preprocess_struct *pars;

	int time_i[7], time_j[5];
	double time_d = 0.0;
	double navlon = 0.0;
	double navlat = 0.0;
	double sensordepth = 0.0;
	double speed = 0.0;
	double heading = 0.0;
	double roll = 0.0;
	double pitch = 0.0;
	double heave = 0.0;
	double altitude = 0.0;
	int interp_status = MB_SUCCESS;
	int interp_error = MB_ERROR_NO_ERROR;
	int jnav = 0;
	int jsensordepth = 0;
	int jheading = 0;
	int jaltitude = 0;
	int jattitude = 0;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:                    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:                   %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:                  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       platform_ptr:               %p\n", (void *)platform_ptr);
		fprintf(stderr, "dbg2       preprocess_pars_ptr:        %p\n", (void *)preprocess_pars_ptr);
	}

	/* always successful */
	status = MB_SUCCESS;
	*error = MB_ERROR_NO_ERROR;

	/* check for non-null data */
	assert(mbio_ptr != NULL);
	assert(store_ptr != NULL);
	assert(preprocess_pars_ptr != NULL);

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointers */
	store = (struct mbsys_jstar_struct *)store_ptr;
	platform = (struct mb_platform_struct *)platform_ptr;
	pars = (struct mb_preprocess_struct *)preprocess_pars_ptr;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       target_sensor:              %d\n", pars->target_sensor);
		fprintf(stderr, "dbg2       timestamp_changed:          %d\n", pars->timestamp_changed);
		fprintf(stderr, "dbg2       time_d:                     %f\n", pars->time_d);
		fprintf(stderr, "dbg2       n_nav:                      %d\n", pars->n_nav);
		fprintf(stderr, "dbg2       nav_time_d:                 %p\n", pars->nav_time_d);
		fprintf(stderr, "dbg2       nav_lon:                    %p\n", pars->nav_lon);
		fprintf(stderr, "dbg2       nav_lat:                    %p\n", pars->nav_lat);
		fprintf(stderr, "dbg2       nav_speed:                  %p\n", pars->nav_speed);
		fprintf(stderr, "dbg2       n_sensordepth:              %d\n", pars->n_sensordepth);
		fprintf(stderr, "dbg2       sensordepth_time_d:         %p\n", pars->sensordepth_time_d);
		fprintf(stderr, "dbg2       sensordepth_sensordepth:    %p\n", pars->sensordepth_sensordepth);
		fprintf(stderr, "dbg2       n_heading:                  %d\n", pars->n_heading);
		fprintf(stderr, "dbg2       heading_time_d:             %p\n", pars->heading_time_d);
		fprintf(stderr, "dbg2       heading_heading:            %p\n", pars->heading_heading);
		fprintf(stderr, "dbg2       n_altitude:                 %d\n", pars->n_altitude);
		fprintf(stderr, "dbg2       altitude_time_d:            %p\n", pars->altitude_time_d);
		fprintf(stderr, "dbg2       altitude_altitude:          %p\n", pars->altitude_altitude);
		fprintf(stderr, "dbg2       n_attitude:                 %d\n", pars->n_attitude);
		fprintf(stderr, "dbg2       attitude_time_d:            %p\n", pars->attitude_time_d);
		fprintf(stderr, "dbg2       attitude_roll:              %p\n", pars->attitude_roll);
		fprintf(stderr, "dbg2       attitude_pitch:             %p\n", pars->attitude_pitch);
		fprintf(stderr, "dbg2       attitude_heave:             %p\n", pars->attitude_heave);
		fprintf(stderr, "dbg2       n_kluge:                    %d\n", pars->n_kluge);
		for (i = 0; i < pars->n_kluge; i++)
			fprintf(stderr, "dbg2       kluge_id[%d]:                    %d\n", i, pars->kluge_id[i]);
	}

	/* preprocess subbottom data */
	if (store->kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* change timestamp if indicated */
		if (pars->timestamp_changed == MB_YES) {
			time_d = pars->time_d;
			mb_get_date(verbose, time_d, time_i);
			mb_get_jtime(verbose, time_i, time_j);
			sbp->year = time_i[0];
			sbp->day = time_j[1];
			sbp->hour = time_i[3];
			sbp->minute = time_i[4];
			sbp->second = time_i[5];
			sbp->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));
			fprintf(stderr,
			        "Timestamp changed in function %s: "
			        "%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%6.6d "
			        "| ping_number:%d\n",
			        function_name, time_i[0], time_i[1], time_i[2], time_i[3], time_i[4], time_i[5], time_i[6], sbp->pingNum);
		}

		/* get time */
		time_j[0] = sbp->year;
		time_j[1] = sbp->day;
		time_j[2] = 60 * sbp->hour + sbp->minute;
		time_j[3] = sbp->second;
		time_j[4] = (int)1000 * (sbp->millisecondsToday - 1000 * floor(0.001 * ((double)sbp->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, &time_d);
	}

	/* preprocess sidescan data */
	else if (store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* change timestamp if indicated */
		if (pars->timestamp_changed == MB_YES) {
			time_d = pars->time_d;
			mb_get_date(verbose, time_d, time_i);
			mb_get_jtime(verbose, time_i, time_j);
			ssport->year = time_i[0];
			ssport->day = time_j[1];
			ssport->hour = time_i[3];
			ssport->minute = time_i[4];
			ssport->second = time_i[5];
			ssport->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));
			ssstbd->year = time_i[0];
			ssstbd->day = time_j[1];
			ssstbd->hour = time_i[3];
			ssstbd->minute = time_i[4];
			ssstbd->second = time_i[5];
			ssstbd->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));
			fprintf(stderr,
			        "Timestamp changed in function %s: "
			        "%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%6.6d "
			        "| ping_number:%d\n",
			        function_name, time_i[0], time_i[1], time_i[2], time_i[3], time_i[4], time_i[5], time_i[6], ssport->pingNum);
		}

		/* get time */
		time_j[0] = ssport->year;
		time_j[1] = ssport->day;
		time_j[2] = 60 * ssport->hour + ssport->minute;
		time_j[3] = ssport->second;
		time_j[4] = (int)1000 * (ssport->millisecondsToday - 1000 * floor(0.001 * ((double)ssport->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, &time_d);
	}

	if (store->kind == MB_DATA_SUBBOTTOM_SUBBOTTOM || store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2) {
		/* get nav sensordepth heading attitude values for record timestamp */
		if (pars->n_nav > 1) {
			interp_status = mb_linear_interp_longitude(verbose, pars->nav_time_d - 1, pars->nav_lon - 1, pars->n_nav, time_d, &navlon,
		                                    &jnav, &interp_error);
			interp_status = mb_linear_interp_latitude(verbose, pars->nav_time_d - 1, pars->nav_lat - 1, pars->n_nav, time_d, &navlat,
		                                    &jnav, &interp_error);
			interp_status = mb_linear_interp(verbose, pars->nav_time_d - 1, pars->nav_speed - 1, pars->n_nav, time_d, &speed,
											&jnav, &interp_error);
		}

		/* interpolate sensordepth */
		if (pars->n_sensordepth > 1) {
			interp_status = mb_linear_interp(verbose, pars->sensordepth_time_d - 1, pars->sensordepth_sensordepth - 1,
		                                 pars->n_sensordepth, time_d, &sensordepth, &jsensordepth, &interp_error);
		}

		/* interpolate heading */
		if (pars->n_heading > 1) {
			interp_status = mb_linear_interp_heading(verbose, pars->heading_time_d - 1, pars->heading_heading - 1, pars->n_heading,
		                                         time_d, &heading, &jheading, &interp_error);
		}

		/* interpolate altitude */
		if (pars->n_altitude > 1) {
			interp_status = mb_linear_interp(verbose, pars->altitude_time_d - 1, pars->altitude_altitude - 1, pars->n_altitude,
		                                 time_d, &altitude, &jaltitude, &interp_error);
		}

		/* interpolate attitude */
		if (pars->n_attitude > 1) {
			interp_status = mb_linear_interp(verbose, pars->attitude_time_d - 1, pars->attitude_roll - 1, pars->n_attitude, time_d,
											 &roll, &jattitude, &interp_error);
			interp_status = mb_linear_interp(verbose, pars->attitude_time_d - 1, pars->attitude_pitch - 1, pars->n_attitude, time_d,
											 &pitch, &jattitude, &interp_error);
			interp_status = mb_linear_interp(verbose, pars->attitude_time_d - 1, pars->attitude_heave - 1, pars->n_attitude, time_d,
											 &heave, &jattitude, &interp_error);
		}
	}

	/* preprocess subbottom data */
	if (store->kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* set navigation */
		if (pars->n_nav > 1) {
			if (navlon < 180.0)
				navlon = navlon + 360.0;
			if (navlon > 180.0)
				navlon = navlon - 360.0;
			sbp->coordX = (int)(600000.0 * navlon);
			sbp->coordX = (int)(600000.0 * navlat);
			sbp->coordUnits = 2;
		}

		/* set heading */
		if (pars->n_heading > 1) {
			if (heading > 180.0)
				heading -= 360.0;
			if (heading < -180.0)
				heading += 360.0;
			sbp->heading = (short)(100.0 * heading);
		}

		/* set sonardepth */
		if (pars->n_sensordepth > 1) {
			sbp->startDepth = sensordepth / sbp->sampleInterval / 0.00000075;
			sbp->sonarDepth = 1000 * sensordepth;
		}
		
		/* set altitude */
		if (pars->n_altitude > 1) {
			sbp->sonarAltitude = (int)(1000 * altitude);
		}

		/* set attitude */
		if (pars->n_attitude > 1) {
			sbp->roll = 32768 * roll / 180.0;
			sbp->pitch = 32768 * pitch / 180.0;
		}
	}

	/* preprocess sidescan data */
	else if (store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* set navigation */
		if (pars->n_nav > 1) {
			if (navlon < 180.0)
				navlon = navlon + 360.0;
			if (navlon > 180.0)
				navlon = navlon - 360.0;
			ssport->coordX = 600000.0 * navlon;
			ssport->coordY = 600000.0 * navlat;
			ssport->coordUnits = 2;
			ssstbd->coordX = 600000.0 * navlon;
			ssstbd->coordY = 600000.0 * navlat;
			ssstbd->coordUnits = 2;
		}

		/* set heading and speed */
		if (pars->n_heading > 1) {
			if (heading > 180.0)
				heading -= 360.0;
			if (heading < -180.0)
				heading += 360.0;
			ssport->heading = (short)(100.0 * heading);
			ssstbd->heading = (short)(100.0 * heading);
		}

		/* set sonardepth */
		if (pars->n_sensordepth > 1) {
			ssport->startDepth = sensordepth / ssport->sampleInterval / 0.00000075;
			ssstbd->startDepth = sensordepth / ssstbd->sampleInterval / 0.00000075;
			ssport->sonarDepth = 1000 * sensordepth;
			ssstbd->sonarDepth = 1000 * sensordepth;
		}
		
		/* set altitude */
		if (pars->n_altitude > 1) {
			ssport->sonarAltitude = (int)(1000 * altitude);
			ssstbd->sonarAltitude = (int)(1000 * altitude);
		}

		/* set attitude */
		if (pars->n_attitude > 1) {
			ssport->roll = 32768 * roll / 180.0;
			ssport->pitch = 32768 * pitch / 180.0;
			ssstbd->roll = 32768 * roll / 180.0;
			ssstbd->pitch = 32768 * pitch / 180.0;
		}
	}

	/* preprocess comment */
	else if (store->kind == MB_DATA_COMMENT) {
	}

	/* preprocess anything else */
	else {
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int time_i[7], double *time_d, double *navlon,
                        double *navlat, double *speed, double *heading, int *nbath, int *namp, int *nss, char *beamflag,
                        double *bath, double *amp, double *bathacrosstrack, double *bathalongtrack, double *ss,
                        double *ssacrosstrack, double *ssalongtrack, char *comment, int *error) {
	char *function_name = "mbsys_jstar_extract";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	int time_j[5];
	double rawpixelsize;
	double pixelsize;
	double range, altitude;
	double weight;
	int istart, jstart;
	int i, j;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract subbottom data from structure */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* get time */
		time_j[0] = sbp->year;
		time_j[1] = sbp->day;
		time_j[2] = 60 * sbp->hour + sbp->minute;
		time_j[3] = sbp->second;
		time_j[4] = (int)1000 * (sbp->millisecondsToday - 1000 * floor(0.001 * ((double)sbp->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, time_d);

		/* get navigation */
		*navlon = sbp->coordX / 600000.0;
		*navlat = sbp->coordY / 600000.0;

		/* get heading */
		*heading = sbp->heading / 100.0;
		if (*heading > 360.0)
			*heading -= 360.0;
		if (*heading < 0.0)
			*heading += 360.0;

		/* get speed */
		*speed = 0.0;

		/* set beamwidths in mb_io structure */
		mb_io_ptr->beamwidth_ltrack = 20.0;
		mb_io_ptr->beamwidth_xtrack = 20.0;

		/* read distance and depth values into storage arrays */
		*nbath = 1;
		*namp = 0;
		*nss = 0;

		/* get nadir depth */
		if (sbp->sonarDepth > 0) {
			bath[0] = 0.001 * sbp->sonarDepth;
			beamflag[0] = MB_FLAG_NONE;
		}
		else if (sbp->sonarDepth < 0) {
			bath[0] = -0.001 * sbp->sonarDepth;
			beamflag[0] = MB_FLAG_MANUAL + MB_FLAG_FLAG;
		}
		else {
			bath[0] = 0.0;
			beamflag[0] = MB_FLAG_NULL;
		}

		/* print debug statements */
		if (verbose >= 5) {
			fprintf(stderr, "\ndbg4  Data extracted by MBIO function <%s>\n", function_name);
			fprintf(stderr, "dbg4  Extracted values:\n");
			fprintf(stderr, "dbg4       kind:       %d\n", *kind);
			fprintf(stderr, "dbg4       error:      %d\n", *error);
			fprintf(stderr, "dbg4       time_i[0]:  %d\n", time_i[0]);
			fprintf(stderr, "dbg4       time_i[1]:  %d\n", time_i[1]);
			fprintf(stderr, "dbg4       time_i[2]:  %d\n", time_i[2]);
			fprintf(stderr, "dbg4       time_i[3]:  %d\n", time_i[3]);
			fprintf(stderr, "dbg4       time_i[4]:  %d\n", time_i[4]);
			fprintf(stderr, "dbg4       time_i[5]:  %d\n", time_i[5]);
			fprintf(stderr, "dbg4       time_i[6]:  %d\n", time_i[6]);
			fprintf(stderr, "dbg4       time_d:     %f\n", *time_d);
			fprintf(stderr, "dbg4       longitude:  %f\n", *navlon);
			fprintf(stderr, "dbg4       latitude:   %f\n", *navlat);
			fprintf(stderr, "dbg4       speed:      %f\n", *speed);
			fprintf(stderr, "dbg4       heading:    %f\n", *heading);
			fprintf(stderr, "dbg4       nbath:      %d\n", *nbath);
			for (i = 0; i < *nbath; i++)
				fprintf(stderr, "dbg4       beam:%4d  flag:%3d  bath:%f  bathdist:%f\n", i, beamflag[i], bath[i],
				        bathacrosstrack[i]);
			fprintf(stderr, "dbg4        nss:      %d\n", *nss);
			for (i = 0; i < *nss; i++)
				fprintf(stderr, "dbg4        beam:%d   ss:%f  acrosstrack:%f  alongtrack:%f\n", i, ss[i], ssacrosstrack[i],
				        ssalongtrack[i]);
		}

		/* done translating values */
	}

	/* extract sidescan data from structure */
	else if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* get time */
		time_j[0] = ssport->year;
		time_j[1] = ssport->day;
		time_j[2] = 60 * ssport->hour + ssport->minute;
		time_j[3] = ssport->second;
		time_j[4] = (int)1000 * (ssport->millisecondsToday - 1000 * floor(0.001 * ((double)ssport->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, time_d);

		/* get navigation */
		*navlon = ssport->coordX / 600000.0;
		*navlat = ssport->coordY / 600000.0;

		/* get heading */
		*heading = ssport->heading / 100.0;
		if (*heading > 360.0)
			*heading -= 360.0;
		if (*heading < 0.0)
			*heading += 360.0;

		/* get speed */
		*speed = 0.0;

		/* set beamwidths in mb_io structure */
		mb_io_ptr->beamwidth_ltrack = 1.5;
		mb_io_ptr->beamwidth_xtrack = 0.1;

		/* read distance and depth values into storage arrays */
		/* average sidescan into a MBSYS_JSTAR_PIXELS_MAX pixel array */
		*nbath = 1;
		*namp = 0;
		if ((ssport->samples + ssstbd->samples) > MBSYS_JSTAR_PIXELS_MAX)
			*nss = MBSYS_JSTAR_PIXELS_MAX;
		else
			*nss = ssport->samples + ssstbd->samples;

		/* get nadir depth */
		if (ssport->sonarDepth > 0) {
			bath[0] = 0.001 * ssport->sonarDepth;
			beamflag[0] = MB_FLAG_NONE;
		}
		else if (ssport->sonarDepth < 0) {
			bath[0] = -0.001 * ssport->sonarDepth;
			beamflag[0] = MB_FLAG_MANUAL + MB_FLAG_FLAG;
		}
		else {
			bath[0] = 0.0;
			beamflag[0] = MB_FLAG_NULL;
		}

		/* get pixel sizes and bottom arrival */
		rawpixelsize = ssport->sampleInterval * 0.00000075;
		if ((ssport->samples + ssstbd->samples) > *nss)
			pixelsize = rawpixelsize * ((double)(ssport->samples + ssstbd->samples)) / ((double)(*nss));
		else
			pixelsize = rawpixelsize;
		altitude = 0.001 * ssport->sonarAltitude;
		// fprintf(stderr,"rawpixelsize:%f pixelsize:%f altitude:%f\n", rawpixelsize,pixelsize,altitude);

		/* zero the array */
		for (i = 0; i < *nss; i++) {
			ss[i] = 0.0;
			range = altitude + fabs(pixelsize * (i - *nss / 2));
			ssacrosstrack[i] = sqrt(range * range - altitude * altitude);
			if (i < *nss / 2)
				ssacrosstrack[i] = -ssacrosstrack[i];
			ssalongtrack[i] = 0.0;
			/*fprintf(stderr,"i:%d range:%f altitude:%f xtrack:%f\n",i,range,altitude,ssacrosstrack[i]);*/
		}

		/* bin the data */
		istart = (int)(altitude / rawpixelsize);
		/*fprintf(stderr,"istart:%d altitude:%f rawpixelsize:%f startDepth:%d\n",
		istart,altitude,rawpixelsize,ssport->startDepth);*/
		weight = exp(MB_LN_2 * ((double)ssport->weightingFactor));
		/*fprintf(stderr, "Subsystem: %d Weights: %d %f ",ssport->message.subsystem,ssport->weightingFactor,weight);*/
		jstart = *nss / 2;
		/*fprintf(stderr,"Port istart:%d of %d    jstart:%d of %d\n",istart,ssport->samples,jstart,*nss);*/
		for (i = istart; i < ssport->samples; i++) {
			range = rawpixelsize * (i + ssport->startDepth);
			j = jstart - (int)((i - istart) * rawpixelsize / pixelsize);
			ss[j] += ssport->trace[i] / weight;
			ssalongtrack[j] += 1.0;
			/*fprintf(stderr,"Binning Port: i:%d j:%d ss:%f\n",i,j,ss[j]/ssalongtrack[j]);*/
		}
		istart = MAX(0, ((int)(altitude / rawpixelsize)));
		weight = exp(MB_LN_2 * ((double)ssstbd->weightingFactor));
		/*fprintf(stderr, "   %d %f\n",ssstbd->weightingFactor,weight);*/
		/*fprintf(stderr,"Stbd istart:%d of %d    jstart:%d of %d\n",istart,ssstbd->samples,jstart,*nss);*/
		for (i = istart; i < ssstbd->samples; i++) {
			range = rawpixelsize * (i + ssstbd->startDepth);
			j = jstart + (int)((i - istart) * rawpixelsize / pixelsize);
			ss[j] += ssstbd->trace[i] / weight;
			ssalongtrack[j] += 1.0;
			/*fprintf(stderr,"Binning Stbd: i:%d j:%d ss:%f\n",i,j,ss[j]/ssalongtrack[j]);*/
		}

		/* average the data in the bins */
		for (i = 0; i < *nss; i++) {
			if (ss[i] > 0.0 && ssalongtrack[i] > 0.0) {
				ss[i] /= ssalongtrack[i];
				ssalongtrack[i] = 0.0;
			}
			else {
				ss[i] = MB_SIDESCAN_NULL;
			}
		}
		for (i = *nss; i < MBSYS_JSTAR_PIXELS_MAX; i++) {
			ss[i] = MB_SIDESCAN_NULL;
		}

		/* print debug statements */
		if (verbose >= 5) {
			fprintf(stderr, "\ndbg4  Data extracted by MBIO function <%s>\n", function_name);
			fprintf(stderr, "dbg4  Extracted values:\n");
			fprintf(stderr, "dbg4       kind:       %d\n", *kind);
			fprintf(stderr, "dbg4       error:      %d\n", *error);
			fprintf(stderr, "dbg4       time_i[0]:  %d\n", time_i[0]);
			fprintf(stderr, "dbg4       time_i[1]:  %d\n", time_i[1]);
			fprintf(stderr, "dbg4       time_i[2]:  %d\n", time_i[2]);
			fprintf(stderr, "dbg4       time_i[3]:  %d\n", time_i[3]);
			fprintf(stderr, "dbg4       time_i[4]:  %d\n", time_i[4]);
			fprintf(stderr, "dbg4       time_i[5]:  %d\n", time_i[5]);
			fprintf(stderr, "dbg4       time_i[6]:  %d\n", time_i[6]);
			fprintf(stderr, "dbg4       time_d:     %f\n", *time_d);
			fprintf(stderr, "dbg4       longitude:  %f\n", *navlon);
			fprintf(stderr, "dbg4       latitude:   %f\n", *navlat);
			fprintf(stderr, "dbg4       speed:      %f\n", *speed);
			fprintf(stderr, "dbg4       heading:    %f\n", *heading);
			fprintf(stderr, "dbg4       nbath:      %d\n", *nbath);
			for (i = 0; i < *nbath; i++)
				fprintf(stderr, "dbg4       beam:%4d  flag:%3d  bath:%f  bathdist:%f\n", i, beamflag[i], bath[i],
				        bathacrosstrack[i]);
			fprintf(stderr, "dbg4        nss:      %d\n", *nss);
			for (i = 0; i < *nss; i++)
				fprintf(stderr, "dbg4        pixel:%d   ss:%f  acrosstrack:%f  alongtrack:%f\n", i, ss[i], ssacrosstrack[i],
				        ssalongtrack[i]);
		}

		/* done translating values */
	}

	/* extract comment from structure */
	else if (*kind == MB_DATA_COMMENT) {
		/* copy comment */
		strcpy(comment, store->comment.comment);

		/* print debug statements */
		if (verbose >= 4) {
			fprintf(stderr, "\ndbg4  New ping read by MBIO function <%s>\n", function_name);
			fprintf(stderr, "dbg4  New ping values:\n");
			fprintf(stderr, "dbg4       error:      %d\n", *error);
			fprintf(stderr, "dbg4       comment:    %s\n", comment);
		}
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
	}
	if (verbose >= 2 && *error <= MB_ERROR_NO_ERROR && *kind == MB_DATA_COMMENT) {
		fprintf(stderr, "dbg2       comment:     \ndbg2       %s\n", comment);
	}
	else if (verbose >= 2 && *error <= MB_ERROR_NO_ERROR && *kind != MB_DATA_COMMENT) {
		fprintf(stderr, "dbg2       time_i[0]:     %d\n", time_i[0]);
		fprintf(stderr, "dbg2       time_i[1]:     %d\n", time_i[1]);
		fprintf(stderr, "dbg2       time_i[2]:     %d\n", time_i[2]);
		fprintf(stderr, "dbg2       time_i[3]:     %d\n", time_i[3]);
		fprintf(stderr, "dbg2       time_i[4]:     %d\n", time_i[4]);
		fprintf(stderr, "dbg2       time_i[5]:     %d\n", time_i[5]);
		fprintf(stderr, "dbg2       time_i[6]:     %d\n", time_i[6]);
		fprintf(stderr, "dbg2       time_d:        %f\n", *time_d);
		fprintf(stderr, "dbg2       longitude:     %f\n", *navlon);
		fprintf(stderr, "dbg2       latitude:      %f\n", *navlat);
		fprintf(stderr, "dbg2       speed:         %f\n", *speed);
		fprintf(stderr, "dbg2       heading:       %f\n", *heading);
	}
	if (verbose >= 2 && *error <= MB_ERROR_NO_ERROR && (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2)) {
		fprintf(stderr, "dbg2       nbath:         %d\n", *nbath);
		for (i = 0; i < *nbath; i++)
			fprintf(stderr, "dbg2       beam:%4d  flag:%3d  bath:%f  bathdist:%f\n", i, beamflag[i], bath[i], bathacrosstrack[i]);
	}
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_insert(int verbose, void *mbio_ptr, void *store_ptr, int kind, int time_i[7], double time_d, double navlon,
                       double navlat, double speed, double heading, int nbath, int namp, int nss, char *beamflag, double *bath,
                       double *amp, double *bathacrosstrack, double *bathalongtrack, double *ss, double *ssacrosstrack,
                       double *ssalongtrack, char *comment, int *error) {
	char *function_name = "mbsys_jstar_insert";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	int time_j[5];
	double weight, altitude, xtrackmax, range, pixelsize, ssmax;
	int istart, jstart, jxtrackmax;
	int shortspersample;
	int nsamples, trace_size;
	int i, j;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:   %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       kind:       %d\n", kind);
	}
	if (verbose >= 2 && (kind == MB_DATA_DATA || kind == MB_DATA_SIDESCAN2 || kind == MB_DATA_NAV)) {
		fprintf(stderr, "dbg2       time_i[0]:  %d\n", time_i[0]);
		fprintf(stderr, "dbg2       time_i[1]:  %d\n", time_i[1]);
		fprintf(stderr, "dbg2       time_i[2]:  %d\n", time_i[2]);
		fprintf(stderr, "dbg2       time_i[3]:  %d\n", time_i[3]);
		fprintf(stderr, "dbg2       time_i[4]:  %d\n", time_i[4]);
		fprintf(stderr, "dbg2       time_i[5]:  %d\n", time_i[5]);
		fprintf(stderr, "dbg2       time_i[6]:  %d\n", time_i[6]);
		fprintf(stderr, "dbg2       time_d:     %f\n", time_d);
		fprintf(stderr, "dbg2       navlon:     %f\n", navlon);
		fprintf(stderr, "dbg2       navlat:     %f\n", navlat);
		fprintf(stderr, "dbg2       speed:      %f\n", speed);
		fprintf(stderr, "dbg2       heading:    %f\n", heading);
	}
	if (verbose >= 2 && (kind == MB_DATA_DATA || kind == MB_DATA_SIDESCAN2)) {
		fprintf(stderr, "dbg2       nbath:      %d\n", nbath);
		if (verbose >= 3)
			for (i = 0; i < nbath; i++)
				fprintf(stderr, "dbg3       beam:%4d  flag:%3d  bath:%f  bathdist:%f\n", i, beamflag[i], bath[i],
				        bathacrosstrack[i]);
		fprintf(stderr, "dbg2       namp:       %d\n", namp);
		if (verbose >= 3)
			for (i = 0; i < namp; i++)
				fprintf(stderr, "dbg3        amp[%d]: %f\n", i, amp[i]);
		fprintf(stderr, "dbg2        nss:       %d\n", nss);
		if (verbose >= 3)
			for (i = 0; i < nss; i++)
				fprintf(stderr, "dbg3        ss[%d]: %f    ssdist[%d]: %f\n", i, ss[i], i, ssacrosstrack[i]);
	}
	if (verbose >= 2 && kind == MB_DATA_COMMENT) {
		fprintf(stderr, "dbg2       comment:     \ndbg2       %s\n", comment);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* set data kind */
	store->kind = kind;

	/* insert subbottom data into structure */
	if (kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* set kind and subsystem */
		store->kind = MB_DATA_SUBBOTTOM_SUBBOTTOM;
		store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SBP;

		/* get time */
		mb_get_jtime(verbose, time_i, time_j);
		sbp->year = time_i[0];
		sbp->day = time_j[1];
		sbp->hour = time_i[3];
		sbp->minute = time_i[4];
		sbp->second = time_i[5];
		sbp->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));

		/* get navigation */
		if (navlon < 180.0)
			navlon = navlon + 360.0;
		if (navlon > 180.0)
			navlon = navlon - 360.0;
		sbp->coordX = (int)(600000.0 * navlon);
		sbp->coordY = (int)(600000.0 * navlat);
		sbp->coordX = (int)(600000.0 * navlon);
		sbp->coordY = (int)(600000.0 * navlat);

		/* get heading */
		if (heading > 180.0)
			heading -= 360.0;
		if (heading < -180.0)
			heading += 360.0;
		sbp->heading = (short)(100.0 * heading);

		/* read distance and depth values into storage arrays */
	}

	/* insert data in structure */
	else if (store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* set kind and subsystem */
		if (ssport->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSLOW &&
		    ssstbd->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSLOW) {
			store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
		}
		else if (ssport->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSHIGH &&
		         ssstbd->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSHIGH) {
			store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
		}
		else if (mb_io_ptr->format == MBF_EDGJSTAR) {
			if (store->kind == MB_DATA_DATA) {
				store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
				ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
				ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
			}
			else {
				store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
				ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
				ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
			}
		}
		else if (mb_io_ptr->format == MBF_EDGJSTR2) {
			if (store->kind == MB_DATA_DATA) {
				store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
				ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
				ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
			}
			else {
				store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
				ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
				ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
			}
		}
		else {
			ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
			ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
		}

		/* get time */
		mb_get_jtime(verbose, time_i, time_j);
		ssport->year = time_i[0];
		ssport->day = time_j[1];
		ssport->hour = time_i[3];
		ssport->minute = time_i[4];
		ssport->second = time_i[5];
		ssport->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));
		ssstbd->year = time_i[0];
		ssstbd->day = time_j[1];
		ssstbd->hour = time_i[3];
		ssstbd->minute = time_i[4];
		ssstbd->second = time_i[5];
		ssstbd->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));

		/* get navigation */
		if (navlon < 180.0)
			navlon = navlon + 360.0;
		if (navlon > 180.0)
			navlon = navlon - 360.0;
		ssport->coordX = 600000.0 * navlon;
		ssport->coordY = 600000.0 * navlat;
		ssstbd->coordX = 600000.0 * navlon;
		ssstbd->coordY = 600000.0 * navlat;
		ssport->coordX = 600000.0 * navlon;
		ssport->coordY = 600000.0 * navlat;
		ssstbd->coordX = 600000.0 * navlon;
		ssstbd->coordY = 600000.0 * navlat;

		/* get heading and speed */
		if (heading > 180.0)
			heading -= 360.0;
		if (heading < -180.0)
			heading += 360.0;
		ssport->heading = (short)(100.0 * heading);
		ssstbd->heading = (short)(100.0 * heading);

		/* put distance and depth values
		    into data structure */

		/* get nadir depth */
		if (nbath > 0) {
			ssport->sonarDepth = 1000 * bath[nbath / 2];
			if (beamflag[nbath / 2] == MB_FLAG_NULL)
				ssport->sonarDepth = 0;
			else if (mb_beam_check_flag(beamflag[nbath / 2]))
				ssport->sonarDepth = -ssport->sonarDepth;
		}

		/* get lateral pixel size */
		altitude = 0.001 * ssport->sonarAltitude;
		xtrackmax = 0.0;
		jxtrackmax = nss / 2;
		pixelsize = 2.0 * altitude / nss;
		nsamples = nss / 2;
		range = altitude;
		for (j = 0; j < nss; j++) {
			if (xtrackmax < fabs(ssacrosstrack[j])) {
				xtrackmax = fabs(ssacrosstrack[j]);
				jxtrackmax = j;
			}
		}
		if (altitude >= 0.0 && xtrackmax >= 0.0 && jxtrackmax != nss / 2) {
			range = sqrt(xtrackmax * xtrackmax + altitude * altitude);
			pixelsize = (range - altitude) / (abs(jxtrackmax - nss / 2));
			nsamples = (int)MIN((double)(nss / 2), (range / pixelsize));
		}
		else {
			status = MB_FAILURE;
			*error = MB_ERROR_UNINTELLIGIBLE;
		}

		/* allocate memory for the trace */
		if (status == MB_SUCCESS) {
			ssport->dataFormat = 0;
			ssstbd->dataFormat = 0;
			shortspersample = 2;
			trace_size = shortspersample * nsamples * sizeof(short);
			if (ssport->trace_alloc < trace_size) {
				if ((status = mb_reallocd(verbose, __FILE__, __LINE__, trace_size, (void **)&(ssport->trace), error)) ==
				    MB_SUCCESS) {
					ssport->trace_alloc = trace_size;
				}
			}
			if (ssstbd->trace_alloc < trace_size) {
				if ((status = mb_reallocd(verbose, __FILE__, __LINE__, trace_size, (void **)&(ssstbd->trace), error)) ==
				    MB_SUCCESS) {
					ssstbd->trace_alloc = trace_size;
				}
			}
		}

		/* put sidescan values
		    into data structure */
		if (status == MB_SUCCESS) {
			/* reset sample interval */
			ssport->sampleInterval = (int)(1000000000.0 * pixelsize / 750.0);
			ssport->startDepth = (int)((0.001 * ssport->sonarDepth) / pixelsize);
			ssport->samples = nsamples;
			ssstbd->sampleInterval = (int)(1000000000.0 * pixelsize / 750.0);
			ssstbd->startDepth = (int)((0.001 * ssstbd->sonarDepth) / pixelsize);
			ssstbd->samples = nsamples;

			/* zero trace before bottom arrival */
			istart = (int)(altitude / pixelsize);
			for (i = 0; i < istart; i++) {
				ssport->trace[i] = 0;
				ssstbd->trace[i] = 0;
			}

			/* get maximum value to determine scaling */
			ssmax = 0.0;
			for (i = 0; i < nss; i++)
				ssmax = MAX(ssmax, ss[i]);
			if (ssmax > 0.0) {
				weight = 65535.0 / ssmax;
				ssport->weightingFactor = log(weight) / MB_LN_2;
				ssstbd->weightingFactor = ssport->weightingFactor;
			}

			/* insert port and starboard traces from sidescan swath */
			jstart = nss / 2 - 1;
			weight = exp(MB_LN_2 * ((double)ssport->weightingFactor));
			for (j = jstart; j >= 0; j--) {
				i = istart + (jstart - j);
				ssport->trace[i] = (short)(ss[j] * weight);
			}
			jstart = nss / 2;
			weight = exp(MB_LN_2 * ((double)ssstbd->weightingFactor));
			for (j = jstart; j < nss; j++) {
				i = istart + (j - jstart);
				ssstbd->trace[i] = (short)(ss[j] * weight);
			}
		}
	}

	/* insert comment in structure */
	else if (store->kind == MB_DATA_COMMENT) {
		strcpy(store->comment.comment, comment);
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return value:\n");
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:  %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_ttimes(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int *nbeams, double *ttimes, double *angles,
                       double *angles_forward, double *angles_null, double *heave, double *alongtrack_offset, double *draft,
                       double *ssv, int *error) {
	char *function_name = "mbsys_jstar_ttimes";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       ttimes:     %p\n", (void *)ttimes);
		fprintf(stderr, "dbg2       angles_xtrk:%p\n", (void *)angles);
		fprintf(stderr, "dbg2       angles_ltrk:%p\n", (void *)angles_forward);
		fprintf(stderr, "dbg2       angles_null:%p\n", (void *)angles_null);
		fprintf(stderr, "dbg2       heave:      %p\n", (void *)heave);
		fprintf(stderr, "dbg2       ltrk_off:   %p\n", (void *)alongtrack_offset);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract data from structure */
	if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get nbeams */
		*nbeams = 0;
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
	}
	if (verbose >= 2 && *error == MB_ERROR_NO_ERROR) {
		fprintf(stderr, "dbg2       draft:      %f\n", *draft);
		fprintf(stderr, "dbg2       ssv:        %f\n", *ssv);
		fprintf(stderr, "dbg2       nbeams:     %d\n", *nbeams);
		for (i = 0; i < *nbeams; i++)
			fprintf(stderr, "dbg2       beam %d: tt:%f  angle_xtrk:%f  angle_ltrk:%f  angle_null:%f  depth_off:%f  ltrk_off:%f\n",
			        i, ttimes[i], angles[i], angles_forward[i], angles_null[i], heave[i], alongtrack_offset[i]);
	}
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_detects(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int *nbeams, int *detects, int *error) {
	char *function_name = "mbsys_jstar_detects";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       detects:    %p\n", (void *)detects);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract data from structure */
	if (*kind == MB_DATA_DATA) {
		/* get nbeams */
		*nbeams = 0;

		/* set status */
		*error = MB_ERROR_NO_ERROR;
		status = MB_SUCCESS;

		/* done translating values */
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
	}
	if (verbose >= 2 && *error == MB_ERROR_NO_ERROR) {
		fprintf(stderr, "dbg2       nbeams:     %d\n", *nbeams);
		for (i = 0; i < *nbeams; i++)
			fprintf(stderr, "dbg2       beam %d: detects:%d\n", i, detects[i]);
	}
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_altitude(int verbose, void *mbio_ptr, void *store_ptr, int *kind, double *transducer_depth,
                                 double *altitude, int *error) {
	char *function_name = "mbsys_jstar_extract_altitude";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract data from structure */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* get transducer_depth */
		if (sbp->sonarDepth > 0)
			*transducer_depth = 0.001 * sbp->sonarDepth;
		else
			*transducer_depth =
			    sbp->startDepth * sbp->sampleInterval * 0.00000075;
		*altitude = 0.001 * sbp->sonarAltitude;
	}

	else if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get channel */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);

		/* get transducer_depth */
		if (ssport->sonarDepth > 0)
			*transducer_depth = 0.001 * ssport->sonarDepth;
		else
			*transducer_depth = ssport->startDepth * ssport->sampleInterval * 0.00000075;
		*altitude = 0.001 * ssport->sonarAltitude;

		/* set status */
		*error = MB_ERROR_NO_ERROR;
		status = MB_SUCCESS;

		/* done translating values */
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       transducer_depth:  %f\n", *transducer_depth);
		fprintf(stderr, "dbg2       altitude:          %f\n", *altitude);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_insert_altitude(int verbose, void *mbio_ptr, void *store_ptr, double transducer_depth, double altitude,
                                int *error) {
	char *function_name = "mbsys_jstar_insert_altitude";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	int kind;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       transducer_depth:  %f\n", transducer_depth);
		fprintf(stderr, "dbg2       altitude:          %f\n", altitude);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	kind = store->kind;

	/* extract data from structure */
	if (kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* get transducer_depth and altitude */
		sbp->sonarDepth = 1000 * transducer_depth;
		sbp->sonarAltitude = 1000 * altitude;
	}

	else if (kind == MB_DATA_DATA || kind == MB_DATA_SIDESCAN2) {
		/* get channel */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* get transducer_depth and altitude */
		ssport->sonarDepth = 1000 * transducer_depth;
		ssport->sonarAltitude = 1000 * altitude;
		ssstbd->sonarDepth = 1000 * transducer_depth;
		ssstbd->sonarAltitude = 1000 * altitude;

		/* set status */
		*error = MB_ERROR_NO_ERROR;
		status = MB_SUCCESS;

		/* done translating values */
	}

	/* deal with comment */
	else if (kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_nav(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int time_i[7], double *time_d,
                            double *navlon, double *navlat, double *speed, double *heading, double *draft, double *roll,
                            double *pitch, double *heave, int *error) {
	char *function_name = "mbsys_jstar_extract_nav";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	int time_j[5];

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract subbottom data from structure */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* get time */
		time_j[0] = sbp->year;
		time_j[1] = sbp->day;
		time_j[2] = 60 * sbp->hour + sbp->minute;
		time_j[3] = sbp->second;
		time_j[4] = (int)1000 * (sbp->millisecondsToday - 1000 * floor(0.001 * ((double)sbp->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, time_d);

		/* get navigation */
		*navlon = sbp->coordX / 600000.0;
		*navlat = sbp->coordY / 600000.0;

		/* get heading */
		*heading = sbp->heading / 100.0;
		if (*heading > 360.0)
			*heading -= 360.0;
		if (*heading < 0.0)
			*heading += 360.0;

		/* get speed */
		*speed = 0.0;

		/* get draft */
		if (sbp->sonarDepth > 0)
			*draft = 0.001 * sbp->sonarDepth;
		else
			*draft = sbp->startDepth * sbp->sampleInterval * 0.00000075;

		/* get attitude */
		*roll = 180.0 / 32768.0 * (double)sbp->roll;
		*pitch = 180.0 / 32768.0 * (double)sbp->pitch;
		*heave = 0.0;

		/* print debug statements */
		if (verbose >= 5) {
			fprintf(stderr, "\ndbg4  Data extracted by MBIO function <%s>\n", function_name);
			fprintf(stderr, "dbg4  Extracted values:\n");
			fprintf(stderr, "dbg4       kind:       %d\n", *kind);
			fprintf(stderr, "dbg4       error:      %d\n", *error);
			fprintf(stderr, "dbg4       time_i[0]:  %d\n", time_i[0]);
			fprintf(stderr, "dbg4       time_i[1]:  %d\n", time_i[1]);
			fprintf(stderr, "dbg4       time_i[2]:  %d\n", time_i[2]);
			fprintf(stderr, "dbg4       time_i[3]:  %d\n", time_i[3]);
			fprintf(stderr, "dbg4       time_i[4]:  %d\n", time_i[4]);
			fprintf(stderr, "dbg4       time_i[5]:  %d\n", time_i[5]);
			fprintf(stderr, "dbg4       time_i[6]:  %d\n", time_i[6]);
			fprintf(stderr, "dbg4       time_d:     %f\n", *time_d);
			fprintf(stderr, "dbg4       longitude:  %f\n", *navlon);
			fprintf(stderr, "dbg4       latitude:   %f\n", *navlat);
			fprintf(stderr, "dbg4       speed:      %f\n", *speed);
			fprintf(stderr, "dbg4       heading:    %f\n", *heading);
			fprintf(stderr, "dbg4       draft:      %f\n", *draft);
			fprintf(stderr, "dbg4       roll:       %f\n", *roll);
			fprintf(stderr, "dbg4       pitch:      %f\n", *pitch);
			fprintf(stderr, "dbg4       heave:      %f\n", *heave);
		}

		/* done translating values */
	}

	/* extract data from structure */
	else if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get channel */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);

		/* get time */
		time_j[0] = ssport->year;
		time_j[1] = ssport->day;
		time_j[2] = 60 * ssport->hour + ssport->minute;
		time_j[3] = ssport->second;
		time_j[4] = (int)1000 * (ssport->millisecondsToday - 1000 * floor(0.001 * ((double)ssport->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);
		mb_get_time(verbose, time_i, time_d);

		/* get navigation */
		*navlon = ssport->coordX / 600000.0;
		*navlat = ssport->coordY / 600000.0;

		/* get heading */
		*heading = ssport->heading / 100.0;
		if (*heading > 360.0)
			*heading -= 360.0;
		if (*heading < 0.0)
			*heading += 360.0;

		/* get speed */
		*speed = 0.0;

		/* get draft */
		if (ssport->sonarDepth > 0)
			*draft = 0.001 * ssport->sonarDepth;
		else
			*draft = ssport->startDepth * ssport->sampleInterval * 0.00000075;

		/* get attitude */
		*roll = 180.0 / 32768.0 * (double)ssport->roll;
		*pitch = 180.0 / 32768.0 * (double)ssport->pitch;
		*heave = 0.0;

		/* print debug statements */
		if (verbose >= 5) {
			fprintf(stderr, "\ndbg4  Data extracted by MBIO function <%s>\n", function_name);
			fprintf(stderr, "dbg4  Extracted values:\n");
			fprintf(stderr, "dbg4       kind:       %d\n", *kind);
			fprintf(stderr, "dbg4       error:      %d\n", *error);
			fprintf(stderr, "dbg4       time_i[0]:  %d\n", time_i[0]);
			fprintf(stderr, "dbg4       time_i[1]:  %d\n", time_i[1]);
			fprintf(stderr, "dbg4       time_i[2]:  %d\n", time_i[2]);
			fprintf(stderr, "dbg4       time_i[3]:  %d\n", time_i[3]);
			fprintf(stderr, "dbg4       time_i[4]:  %d\n", time_i[4]);
			fprintf(stderr, "dbg4       time_i[5]:  %d\n", time_i[5]);
			fprintf(stderr, "dbg4       time_i[6]:  %d\n", time_i[6]);
			fprintf(stderr, "dbg4       time_d:     %f\n", *time_d);
			fprintf(stderr, "dbg4       longitude:  %f\n", *navlon);
			fprintf(stderr, "dbg4       latitude:   %f\n", *navlat);
			fprintf(stderr, "dbg4       speed:      %f\n", *speed);
			fprintf(stderr, "dbg4       heading:    %f\n", *heading);
			fprintf(stderr, "dbg4       draft:      %f\n", *draft);
			fprintf(stderr, "dbg4       roll:       %f\n", *roll);
			fprintf(stderr, "dbg4       pitch:      %f\n", *pitch);
			fprintf(stderr, "dbg4       heave:      %f\n", *heave);
		}

		/* done translating values */
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
	}
	if (verbose >= 2 && *error <= MB_ERROR_NO_ERROR && *kind != MB_DATA_COMMENT) {
		fprintf(stderr, "dbg2       time_i[0]:     %d\n", time_i[0]);
		fprintf(stderr, "dbg2       time_i[1]:     %d\n", time_i[1]);
		fprintf(stderr, "dbg2       time_i[2]:     %d\n", time_i[2]);
		fprintf(stderr, "dbg2       time_i[3]:     %d\n", time_i[3]);
		fprintf(stderr, "dbg2       time_i[4]:     %d\n", time_i[4]);
		fprintf(stderr, "dbg2       time_i[5]:     %d\n", time_i[5]);
		fprintf(stderr, "dbg2       time_i[6]:     %d\n", time_i[6]);
		fprintf(stderr, "dbg2       time_d:        %f\n", *time_d);
		fprintf(stderr, "dbg2       longitude:     %f\n", *navlon);
		fprintf(stderr, "dbg2       latitude:      %f\n", *navlat);
		fprintf(stderr, "dbg2       speed:         %f\n", *speed);
		fprintf(stderr, "dbg2       heading:       %f\n", *heading);
		fprintf(stderr, "dbg2       draft:         %f\n", *draft);
		fprintf(stderr, "dbg2       roll:          %f\n", *roll);
		fprintf(stderr, "dbg2       pitch:         %f\n", *pitch);
		fprintf(stderr, "dbg2       heave:         %f\n", *heave);
	}
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_insert_nav(int verbose, void *mbio_ptr, void *store_ptr, int time_i[7], double time_d, double navlon,
                           double navlat, double speed, double heading, double draft, double roll, double pitch, double heave,
                           int *error) {
	char *function_name = "mbsys_jstar_insert_nav";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	int time_j[5];

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:   %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       time_i[0]:  %d\n", time_i[0]);
		fprintf(stderr, "dbg2       time_i[1]:  %d\n", time_i[1]);
		fprintf(stderr, "dbg2       time_i[2]:  %d\n", time_i[2]);
		fprintf(stderr, "dbg2       time_i[3]:  %d\n", time_i[3]);
		fprintf(stderr, "dbg2       time_i[4]:  %d\n", time_i[4]);
		fprintf(stderr, "dbg2       time_i[5]:  %d\n", time_i[5]);
		fprintf(stderr, "dbg2       time_i[6]:  %d\n", time_i[6]);
		fprintf(stderr, "dbg2       time_d:     %f\n", time_d);
		fprintf(stderr, "dbg2       navlon:     %f\n", navlon);
		fprintf(stderr, "dbg2       navlat:     %f\n", navlat);
		fprintf(stderr, "dbg2       speed:      %f\n", speed);
		fprintf(stderr, "dbg2       heading:    %f\n", heading);
		fprintf(stderr, "dbg2       draft:      %f\n", draft);
		fprintf(stderr, "dbg2       roll:       %f\n", roll);
		fprintf(stderr, "dbg2       pitch:      %f\n", pitch);
		fprintf(stderr, "dbg2       heave:      %f\n", heave);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* insert subbottom data into structure */
	if (store->kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* set kind and subsystem */
		store->kind = MB_DATA_SUBBOTTOM_SUBBOTTOM;
		store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SBP;

		/* get time */
		mb_get_jtime(verbose, time_i, time_j);
		sbp->year = time_i[0];
		sbp->day = time_j[1];
		sbp->hour = time_i[3];
		sbp->minute = time_i[4];
		sbp->second = time_i[5];
		sbp->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));

		/* get navigation */
		if (navlon < 180.0)
			navlon = navlon + 360.0;
		if (navlon > 180.0)
			navlon = navlon - 360.0;
		sbp->coordX = (int)(600000.0 * navlon);
		sbp->coordY = (int)(600000.0 * navlat);
		sbp->coordX = (int)(600000.0 * navlon);
		sbp->coordY = (int)(600000.0 * navlat);

		/* get heading */
		if (heading > 180.0)
			heading -= 360.0;
		if (heading < -180.0)
			heading += 360.0;
		sbp->heading = (short)(100.0 * heading);

		/* get draft */
		sbp->startDepth = draft / sbp->sampleInterval / 0.00000075;
		sbp->sonarDepth = 1000 * draft;

		/* get attitude */
		sbp->roll = 32768 * roll / 180.0;
		sbp->pitch = 32768 * pitch / 180.0;
	}

	/* insert data in structure */
	else if (store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* set kind and subsystem */
		store->kind = MB_DATA_DATA;
		if (ssport->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSLOW &&
		    ssstbd->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSLOW) {
			store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
		}
		else if (ssport->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSHIGH &&
		         ssstbd->message.subsystem == MBSYS_JSTAR_SUBSYSTEM_SSHIGH) {
			store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
		}
		else if (store->subsystem != MBSYS_JSTAR_SUBSYSTEM_SSHIGH) {
			store->subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
			ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
			ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSLOW;
		}
		else {
			ssport->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
			ssstbd->message.subsystem = MBSYS_JSTAR_SUBSYSTEM_SSHIGH;
		}

		/* get time */
		mb_get_jtime(verbose, time_i, time_j);
		ssport->year = time_i[0];
		ssport->day = time_j[1];
		ssport->hour = time_i[3];
		ssport->minute = time_i[4];
		ssport->second = time_i[5];
		ssport->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));
		ssstbd->year = time_i[0];
		ssstbd->day = time_j[1];
		ssstbd->hour = time_i[3];
		ssstbd->minute = time_i[4];
		ssstbd->second = time_i[5];
		ssstbd->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));

		/* get navigation */
		if (navlon < 180.0)
			navlon = navlon + 360.0;
		if (navlon > 180.0)
			navlon = navlon - 360.0;
		ssport->coordX = 600000.0 * navlon;
		ssport->coordY = 600000.0 * navlat;
		ssstbd->coordX = 600000.0 * navlon;
		ssstbd->coordY = 600000.0 * navlat;
		ssport->coordX = 600000.0 * navlon;
		ssport->coordY = 600000.0 * navlat;
		ssstbd->coordX = 600000.0 * navlon;
		ssstbd->coordY = 600000.0 * navlat;

		/* get heading and speed */
		if (heading > 180.0)
			heading -= 360.0;
		if (heading < -180.0)
			heading += 360.0;
		ssport->heading = (short)(100.0 * heading);
		ssstbd->heading = (short)(100.0 * heading);

		/* get draft */
		ssport->startDepth = draft / ssport->sampleInterval / 0.00000075;
		ssstbd->startDepth = draft / ssstbd->sampleInterval / 0.00000075;
		ssport->sonarDepth = 1000 * draft;
		ssstbd->sonarDepth = 1000 * draft;

		/* get attitude */
		ssport->roll = 32768 * roll / 180.0;
		ssport->pitch = 32768 * pitch / 180.0;
		ssstbd->roll = 32768 * roll / 180.0;
		ssstbd->pitch = 32768 * pitch / 180.0;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return value:\n");
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:  %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_rawssdimensions(int verbose, void *mbio_ptr, void *store_ptr, int *kind, double *sample_interval,
                                        int *num_samples_port, int *num_samples_stbd, int *error) {
	char *function_name = "mbsys_jstar_extract_rawssdimensions";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract sidescan data from structure */
	if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* get sample_interval */
		if (ssport != NULL)
			*sample_interval = ssport->sampleInterval;
		else if (ssstbd != NULL)
			*sample_interval = ssstbd->sampleInterval;

		/* get numbers of samples */
		if (ssport != NULL)
			*num_samples_port = ssport->samples;
		else
			*num_samples_port = 0;
		if (ssstbd != NULL)
			*num_samples_stbd = ssstbd->samples;
		else
			*num_samples_stbd = 0;
	}

	/* else not a sidescan record */
	else {
		status = MB_FAILURE;
		*error = MB_ERROR_BAD_KIND;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       sample_interval:   %lf\n", *sample_interval);
		fprintf(stderr, "dbg2       num_samples_port:  %d\n", *num_samples_port);
		fprintf(stderr, "dbg2       num_samples_stbd:  %d\n", *num_samples_stbd);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_rawss(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int *sidescan_type,
                              double *sample_interval, double *beamwidth_xtrack, double *beamwidth_ltrack, int *num_samples_port,
                              double *rawss_port, int *num_samples_stbd, double *rawss_stbd, int *error) {
	char *function_name = "mbsys_jstar_extract_rawss";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	double weight;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract sidescan data from structure */
	if (*kind == MB_DATA_DATA || *kind == MB_DATA_SIDESCAN2) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* sidescan type is always linear */
		*sidescan_type = MB_SIDESCAN_LINEAR;

		/* get sample_interval */
		if (ssport != NULL)
			*sample_interval = 0.000000001 * ssport->sampleInterval;
		else if (ssstbd != NULL)
			*sample_interval = 0.000000001 * ssstbd->sampleInterval;

		/* set beam widths */
		if (ssport != NULL) {
			if (ssport->startFreq < 9000)
				*beamwidth_ltrack = 1.3;
			else if (ssport->startFreq < 15000)
				*beamwidth_ltrack = 0.65;
			else
				*beamwidth_ltrack = 0.26;
			*beamwidth_xtrack = 0.1;
		}
		else if (ssstbd != NULL) {
			if (ssstbd->startFreq < 9000)
				*beamwidth_ltrack = 1.3;
			else if (ssstbd->startFreq < 15000)
				*beamwidth_ltrack = 0.65;
			else
				*beamwidth_ltrack = 0.26;
			*beamwidth_xtrack = 0.1;
		}
		else {
			*beamwidth_ltrack = 0.26;
			*beamwidth_xtrack = 0.1;
		}

		/* get numbers of samples and time series */
		if (ssport != NULL) {
			*num_samples_port = ssport->samples;
			weight = exp(MB_LN_2 * ((double)ssport->weightingFactor));
			for (i = 0; i < *num_samples_port; i++) {
				rawss_port[i] = ssport->trace[i] / weight;
			}
		}
		else
			*num_samples_port = 0;
		if (ssstbd != NULL) {
			*num_samples_stbd = ssstbd->samples;
			weight = exp(MB_LN_2 * ((double)ssstbd->weightingFactor));
			for (i = 0; i < *num_samples_stbd; i++) {
				rawss_stbd[i] = ssstbd->trace[i] / weight;
			}
		}
		else
			*num_samples_stbd = 0;
	}

	/* else not a sidescan record */
	else {
		status = MB_FAILURE;
		*error = MB_ERROR_BAD_KIND;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       sidescan_type:     %d\n", *sidescan_type);
		fprintf(stderr, "dbg2       sample_interval:   %lf\n", *sample_interval);
		fprintf(stderr, "dbg2       beamwidth_xtrack:  %lf\n", *beamwidth_xtrack);
		fprintf(stderr, "dbg2       beamwidth_ltrack:  %lf\n", *beamwidth_ltrack);
		fprintf(stderr, "dbg2       num_samples_port:  %d\n", *num_samples_port);
		for (i = 0; i < *num_samples_port; i++)
			fprintf(stderr, "dbg2       sample: %d  rawss_port:%f\n", i, rawss_port[i]);
		fprintf(stderr, "dbg2       num_samples_stbd:  %d\n", *num_samples_stbd);
		for (i = 0; i < *num_samples_stbd; i++)
			fprintf(stderr, "dbg2       sample: %d  rawss_stbd:%f\n", i, rawss_stbd[i]);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_insert_rawss(int verbose, void *mbio_ptr, void *store_ptr, int kind, int sidescan_type, double sample_interval,
                             double beamwidth_xtrack, double beamwidth_ltrack, int num_samples_port, double *rawss_port,
                             int num_samples_stbd, double *rawss_stbd, int *error) {
	char *function_name = "mbsys_jstar_insert_rawss";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *ssport;
	struct mbsys_jstar_channel_struct *ssstbd;
	double weight;
	size_t data_size;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:           %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:            %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:         %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       kind:              %d\n", kind);
		fprintf(stderr, "dbg2       sidescan_type:     %d\n", sidescan_type);
		fprintf(stderr, "dbg2       sample_interval:   %lf\n", sample_interval);
		fprintf(stderr, "dbg2       beamwidth_xtrack:  %lf\n", beamwidth_xtrack);
		fprintf(stderr, "dbg2       beamwidth_ltrack:  %lf\n", beamwidth_ltrack);
		fprintf(stderr, "dbg2       num_samples_port:  %d\n", num_samples_port);
		for (i = 0; i < num_samples_port; i++)
			fprintf(stderr, "dbg2       sample: %d  rawss_port:%f\n", i, rawss_port[i]);
		fprintf(stderr, "dbg2       num_samples_stbd:  %d\n", num_samples_stbd);
		for (i = 0; i < num_samples_stbd; i++)
			fprintf(stderr, "dbg2       sample: %d  rawss_stbd:%f\n", i, rawss_stbd[i]);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* set data kind */
	store->kind = kind;

	/* insert sidescan data into structure */
	if (kind == store->kind && (store->kind == MB_DATA_DATA || store->kind == MB_DATA_SIDESCAN2)) {
		/* get channels */
		ssport = (struct mbsys_jstar_channel_struct *)&(store->ssport);
		ssstbd = (struct mbsys_jstar_channel_struct *)&(store->ssstbd);

		/* set sample_interval */
		if (ssport != NULL)
			ssport->sampleInterval = 1000000000 * sample_interval;
		if (ssstbd != NULL)
			ssstbd->sampleInterval = 1000000000 * sample_interval;

		/* set beam widths */

		/* set numbers of samples and time series */
		if (ssport != NULL) {
			/* set number of samples */
			ssport->samples = num_samples_port;

			/* allocated memory for samples if needed */
			data_size = sizeof(short) * ssport->samples;
			if (ssport->trace_alloc < data_size) {
				status = mb_reallocd(verbose, __FILE__, __LINE__, data_size, (void **)&(ssport->trace), error);
				if (status == MB_SUCCESS) {
					ssport->trace_alloc = data_size;
				}
				else {
					ssport->trace_alloc = 0;
					ssport->samples = 0;
				}
			}

			/* copy the samples, correcting for weighting */
			weight = exp(MB_LN_2 * ((double)ssport->weightingFactor));
			for (i = 0; i < num_samples_port; i++) {
				ssport->trace[i] = (short)(weight * rawss_port[i]);
			}
		}
		if (ssstbd != NULL) {
			/* set number of samples */
			ssstbd->samples = num_samples_stbd;

			/* allocated memory for samples if needed */
			data_size = sizeof(short) * ssstbd->samples;
			if (ssstbd->trace_alloc < data_size) {
				status = mb_reallocd(verbose, __FILE__, __LINE__, data_size, (void **)&(ssstbd->trace), error);
				if (status == MB_SUCCESS) {
					ssstbd->trace_alloc = data_size;
				}
				else {
					ssstbd->trace_alloc = 0;
					ssstbd->samples = 0;
				}
			}

			/* copy the samples, correcting for weighting */
			weight = exp(MB_LN_2 * ((double)ssstbd->weightingFactor));
			for (i = 0; i < num_samples_stbd; i++) {
				ssstbd->trace[i] = (short)(weight * rawss_stbd[i]);
			}
		}
	}

	/* else not a sidescan record */
	else {
		status = MB_FAILURE;
		*error = MB_ERROR_BAD_KIND;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_segytraceheader(int verbose, void *mbio_ptr, void *store_ptr, int *kind, void *segytraceheader_ptr,
                                        int *error) {
	char *function_name = "mbsys_jstar_extract_segytraceheader";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mb_segytraceheader_struct *mb_segytraceheader_ptr;
	double dsonardepth, dsonaraltitude, dwaterdepth;
	int sonardepth, waterdepth;
	int watersoundspeed;
	float fwatertime;
	double longitude, latitude;
	int time_i[7], time_j[5];
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:        %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:         %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:      %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       kind:           %d\n", *kind);
		fprintf(stderr, "dbg2       segytraceheader_ptr: %p\n", (void *)segytraceheader_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;
	mb_segytraceheader_ptr = (struct mb_segytraceheader_struct *)segytraceheader_ptr;

	/* get data kind */
	*kind = store->kind;

	/* extract data from structure */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);

		/* get time */
		time_j[0] = sbp->year;
		time_j[1] = sbp->day;
		time_j[2] = 60 * sbp->hour + sbp->minute;
		time_j[3] = sbp->second;
		time_j[4] = (int)1000 * (sbp->millisecondsToday - 1000 * floor(0.001 * ((double)sbp->millisecondsToday)));
		mb_get_itime(verbose, time_j, time_i);

		/* get needed values */
		/* get transducer_depth */
		if (sbp->sonarDepth > 0)
			dsonardepth = 0.001 * sbp->sonarDepth;
		else
			dsonardepth = sbp->startDepth * sbp->sampleInterval * 0.00000075;
		dsonaraltitude = 0.001 * sbp->sonarAltitude;
		if (sbp->sonarDepth > 0)
			dwaterdepth = 0.001 * sbp->sonarDepth;
		else
			dwaterdepth = dsonardepth + dsonaraltitude;
		sonardepth = (int)(100 * dsonardepth);
		waterdepth = (int)(100 * dwaterdepth);
		watersoundspeed = 1500;
		fwatertime = 2.0 * dwaterdepth / ((double)watersoundspeed);

		/* get navigation */
		longitude = sbp->coordX / 600000.0;
		latitude = sbp->coordY / 600000.0;

		/* extract the data */
		mb_segytraceheader_ptr->seq_num = sbp->pingNum;
		mb_segytraceheader_ptr->seq_reel = sbp->pingNum;
		mb_segytraceheader_ptr->shot_num = sbp->pingNum;
		mb_segytraceheader_ptr->shot_tr = 1;
		mb_segytraceheader_ptr->espn = 0;
		mb_segytraceheader_ptr->rp_num = sbp->pingNum;
		mb_segytraceheader_ptr->rp_tr = 1;
		mb_segytraceheader_ptr->trc_id = 1;
		mb_segytraceheader_ptr->num_vstk = 0;
		mb_segytraceheader_ptr->cdp_fold = 0;
		mb_segytraceheader_ptr->use = sbp->dataFormat;
		mb_segytraceheader_ptr->range = 0;
		mb_segytraceheader_ptr->grp_elev = -sonardepth;
		mb_segytraceheader_ptr->src_elev = -sonardepth;
		mb_segytraceheader_ptr->src_depth = sonardepth;
		mb_segytraceheader_ptr->grp_datum = 0;
		mb_segytraceheader_ptr->src_datum = 0;
		mb_segytraceheader_ptr->src_wbd = waterdepth;
		mb_segytraceheader_ptr->grp_wbd = waterdepth;
		mb_segytraceheader_ptr->elev_scalar = -100; /* 0.01 m precision for depths */
		mb_segytraceheader_ptr->coord_scalar = -100; /* 0.01 arc second precision for position
		                     = 0.3 m precision at equator */
		mb_segytraceheader_ptr->src_long = (int)(longitude * 600000.0);
		mb_segytraceheader_ptr->src_lat = (int)(latitude * 600000.0);
		mb_segytraceheader_ptr->grp_long = (int)(longitude * 600000.0);
		mb_segytraceheader_ptr->grp_lat = (int)(latitude * 600000.0);
		mb_segytraceheader_ptr->coord_units = 2;
		mb_segytraceheader_ptr->wvel = watersoundspeed;
		mb_segytraceheader_ptr->sbvel = 0;
		mb_segytraceheader_ptr->src_up_vel = 0;
		mb_segytraceheader_ptr->grp_up_vel = 0;
		mb_segytraceheader_ptr->src_static = 0;
		mb_segytraceheader_ptr->grp_static = 0;
		mb_segytraceheader_ptr->tot_static = 0;
		mb_segytraceheader_ptr->laga = 0;
		mb_segytraceheader_ptr->delay_mils = 0;
		mb_segytraceheader_ptr->smute_mils = 0;
		mb_segytraceheader_ptr->emute_mils = 0;
		mb_segytraceheader_ptr->nsamps = sbp->samples;
		mb_segytraceheader_ptr->si_micros = (short)(sbp->sampleInterval / 1000);
		for (i = 0; i < 19; i++)
			mb_segytraceheader_ptr->other_1[i] = 0;
		mb_segytraceheader_ptr->year = time_i[0];
		mb_segytraceheader_ptr->day_of_yr = time_j[1];
		mb_segytraceheader_ptr->hour = time_i[3];
		mb_segytraceheader_ptr->min = time_i[4];
		mb_segytraceheader_ptr->sec = time_i[5];
		mb_segytraceheader_ptr->mils = time_i[6] / 1000;
		mb_segytraceheader_ptr->tr_weight = 1;
		for (i = 0; i < 5; i++)
			mb_segytraceheader_ptr->other_2[i] = 0;
		mb_segytraceheader_ptr->delay = 0.0;
		mb_segytraceheader_ptr->smute_sec = 0.0;
		mb_segytraceheader_ptr->emute_sec = 0.0;
		mb_segytraceheader_ptr->si_secs = 0.000000001 * ((float)sbp->sampleInterval);
		mb_segytraceheader_ptr->wbt_secs = fwatertime;
		mb_segytraceheader_ptr->end_of_rp = 0;
		mb_segytraceheader_ptr->dummy1 = 0.0;
		mb_segytraceheader_ptr->dummy2 = 0.0;
		mb_segytraceheader_ptr->dummy3 = 0.0;
		mb_segytraceheader_ptr->dummy4 = 0.0;
		mb_segytraceheader_ptr->soundspeed = watersoundspeed;
		mb_segytraceheader_ptr->distance = 0.0;
		mb_segytraceheader_ptr->roll = 180.0 / 32768.0 * (double)sbp->roll;
		mb_segytraceheader_ptr->pitch = 180.0 / 32768.0 * (double)sbp->pitch;
		mb_segytraceheader_ptr->heading = sbp->heading / 60.0;

		/* done translating values */
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       seq_num:           %d\n", mb_segytraceheader_ptr->seq_num);
		fprintf(stderr, "dbg2       seq_reel:          %d\n", mb_segytraceheader_ptr->seq_reel);
		fprintf(stderr, "dbg2       shot_num:          %d\n", mb_segytraceheader_ptr->shot_num);
		fprintf(stderr, "dbg2       shot_tr:           %d\n", mb_segytraceheader_ptr->shot_tr);
		fprintf(stderr, "dbg2       espn:              %d\n", mb_segytraceheader_ptr->espn);
		fprintf(stderr, "dbg2       rp_num:            %d\n", mb_segytraceheader_ptr->rp_num);
		fprintf(stderr, "dbg2       rp_tr:             %d\n", mb_segytraceheader_ptr->rp_tr);
		fprintf(stderr, "dbg2       trc_id:            %d\n", mb_segytraceheader_ptr->trc_id);
		fprintf(stderr, "dbg2       num_vstk:          %d\n", mb_segytraceheader_ptr->num_vstk);
		fprintf(stderr, "dbg2       cdp_fold:          %d\n", mb_segytraceheader_ptr->cdp_fold);
		fprintf(stderr, "dbg2       use:               %d\n", mb_segytraceheader_ptr->use);
		fprintf(stderr, "dbg2       range:             %d\n", mb_segytraceheader_ptr->range);
		fprintf(stderr, "dbg2       grp_elev:          %d\n", mb_segytraceheader_ptr->grp_elev);
		fprintf(stderr, "dbg2       src_elev:          %d\n", mb_segytraceheader_ptr->src_elev);
		fprintf(stderr, "dbg2       src_depth:         %d\n", mb_segytraceheader_ptr->src_depth);
		fprintf(stderr, "dbg2       grp_datum:         %d\n", mb_segytraceheader_ptr->grp_datum);
		fprintf(stderr, "dbg2       src_datum:         %d\n", mb_segytraceheader_ptr->src_datum);
		fprintf(stderr, "dbg2       src_wbd:           %d\n", mb_segytraceheader_ptr->src_wbd);
		fprintf(stderr, "dbg2       grp_wbd:           %d\n", mb_segytraceheader_ptr->grp_wbd);
		fprintf(stderr, "dbg2       elev_scalar:       %d\n", mb_segytraceheader_ptr->elev_scalar);
		fprintf(stderr, "dbg2       coord_scalar:      %d\n", mb_segytraceheader_ptr->coord_scalar);
		fprintf(stderr, "dbg2       src_long:          %d\n", mb_segytraceheader_ptr->src_long);
		fprintf(stderr, "dbg2       src_lat:           %d\n", mb_segytraceheader_ptr->src_lat);
		fprintf(stderr, "dbg2       grp_long:          %d\n", mb_segytraceheader_ptr->grp_long);
		fprintf(stderr, "dbg2       grp_lat:           %d\n", mb_segytraceheader_ptr->grp_lat);
		fprintf(stderr, "dbg2       coord_units:       %d\n", mb_segytraceheader_ptr->coord_units);
		fprintf(stderr, "dbg2       wvel:              %d\n", mb_segytraceheader_ptr->wvel);
		fprintf(stderr, "dbg2       sbvel:             %d\n", mb_segytraceheader_ptr->sbvel);
		fprintf(stderr, "dbg2       src_up_vel:        %d\n", mb_segytraceheader_ptr->src_up_vel);
		fprintf(stderr, "dbg2       grp_up_vel:        %d\n", mb_segytraceheader_ptr->grp_up_vel);
		fprintf(stderr, "dbg2       src_static:        %d\n", mb_segytraceheader_ptr->src_static);
		fprintf(stderr, "dbg2       grp_static:        %d\n", mb_segytraceheader_ptr->grp_static);
		fprintf(stderr, "dbg2       tot_static:        %d\n", mb_segytraceheader_ptr->tot_static);
		fprintf(stderr, "dbg2       laga:              %d\n", mb_segytraceheader_ptr->laga);
		fprintf(stderr, "dbg2       delay_mils:        %d\n", mb_segytraceheader_ptr->delay_mils);
		fprintf(stderr, "dbg2       smute_mils:        %d\n", mb_segytraceheader_ptr->smute_mils);
		fprintf(stderr, "dbg2       emute_mils:        %d\n", mb_segytraceheader_ptr->emute_mils);
		fprintf(stderr, "dbg2       nsamps:            %d\n", mb_segytraceheader_ptr->nsamps);
		fprintf(stderr, "dbg2       si_micros:         %d\n", mb_segytraceheader_ptr->si_micros);
		for (i = 0; i < 19; i++)
			fprintf(stderr, "dbg2       other_1[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_1[i]);
		fprintf(stderr, "dbg2       year:              %d\n", mb_segytraceheader_ptr->year);
		fprintf(stderr, "dbg2       day_of_yr:         %d\n", mb_segytraceheader_ptr->day_of_yr);
		fprintf(stderr, "dbg2       hour:              %d\n", mb_segytraceheader_ptr->hour);
		fprintf(stderr, "dbg2       min:               %d\n", mb_segytraceheader_ptr->min);
		fprintf(stderr, "dbg2       sec:               %d\n", mb_segytraceheader_ptr->sec);
		fprintf(stderr, "dbg2       mils:              %d\n", mb_segytraceheader_ptr->mils);
		fprintf(stderr, "dbg2       tr_weight:         %d\n", mb_segytraceheader_ptr->tr_weight);
		for (i = 0; i < 5; i++)
			fprintf(stderr, "dbg2       other_2[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_2[i]);
		fprintf(stderr, "dbg2       delay:             %f\n", mb_segytraceheader_ptr->delay);
		fprintf(stderr, "dbg2       smute_sec:         %f\n", mb_segytraceheader_ptr->smute_sec);
		fprintf(stderr, "dbg2       emute_sec:         %f\n", mb_segytraceheader_ptr->emute_sec);
		fprintf(stderr, "dbg2       si_secs:           %f\n", mb_segytraceheader_ptr->si_secs);
		fprintf(stderr, "dbg2       wbt_secs:          %f\n", mb_segytraceheader_ptr->wbt_secs);
		fprintf(stderr, "dbg2       end_of_rp:         %d\n", mb_segytraceheader_ptr->end_of_rp);
		fprintf(stderr, "dbg2       dummy1:            %f\n", mb_segytraceheader_ptr->dummy1);
		fprintf(stderr, "dbg2       dummy2:            %f\n", mb_segytraceheader_ptr->dummy2);
		fprintf(stderr, "dbg2       dummy3:            %f\n", mb_segytraceheader_ptr->dummy3);
		fprintf(stderr, "dbg2       dummy4:            %f\n", mb_segytraceheader_ptr->dummy4);
		fprintf(stderr, "dbg2       soundspeed:        %f\n", mb_segytraceheader_ptr->soundspeed);
		fprintf(stderr, "dbg2       distance:          %f\n", mb_segytraceheader_ptr->distance);
		fprintf(stderr, "dbg2       roll:              %f\n", mb_segytraceheader_ptr->roll);
		fprintf(stderr, "dbg2       pitch:             %f\n", mb_segytraceheader_ptr->pitch);
		fprintf(stderr, "dbg2       heading:           %f\n", mb_segytraceheader_ptr->heading);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_extract_segy(int verbose, void *mbio_ptr, void *store_ptr, int *sampleformat, int *kind, void *segyheader_ptr,
                             float *segydata, int *error) {
	char *function_name = "mbsys_jstar_extract_segy";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mb_segytraceheader_struct *mb_segytraceheader_ptr;
	short *shortptr;
	unsigned short *ushortptr;
	double weight;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:           %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:            %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:         %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       sampleformat:      %d\n", *sampleformat);
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       segyheader_ptr:    %p\n", (void *)segyheader_ptr);
		fprintf(stderr, "dbg2       segydata:          %p\n", (void *)segydata);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	*kind = store->kind;

	/* get segy traceheader */
	mb_segytraceheader_ptr = (struct mb_segytraceheader_struct *)segyheader_ptr;

	/* extract data from structure */
	if (*kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);
		shortptr = (short *)sbp->trace;
		ushortptr = (unsigned short *)sbp->trace;

		/* extract segy header */
		status = mbsys_jstar_extract_segytraceheader(verbose, mbio_ptr, store_ptr, kind, segyheader_ptr, error);

		/* get the trace weight */
		weight = exp(MB_LN_2 * ((double)sbp->weightingFactor));
		/*fprintf(stderr, "Subsystem: %d Weight: %d %f\n",sbp->message.subsystem,sbp->weightingFactor,weight);*/

		/* extract the data */
		if (sbp->dataFormat == MBSYS_JSTAR_TRACEFORMAT_ENVELOPE) {
			*sampleformat = MB_SEGY_SAMPLEFORMAT_ENVELOPE;
			for (i = 0; i < sbp->samples; i++) {
				segydata[i] = (float)(((double)ushortptr[i]) / weight);
			}
		}
		else if (sbp->dataFormat == MBSYS_JSTAR_TRACEFORMAT_ANALYTIC) {
			/* if no format specified do envelope by default */
			if (*sampleformat == MB_SEGY_SAMPLEFORMAT_NONE)
				*sampleformat = MB_SEGY_SAMPLEFORMAT_ENVELOPE;

			/* convert analytic data to desired envelope */
			if (*sampleformat == MB_SEGY_SAMPLEFORMAT_ENVELOPE) {
				for (i = 0; i < sbp->samples; i++) {
					segydata[i] =
					    (float)(sqrt((double)(shortptr[2 * i] * shortptr[2 * i] + shortptr[2 * i + 1] * shortptr[2 * i + 1])) /
					            weight);
				}
			}

			/* else extract desired analytic data */
			else if (*sampleformat == MB_SEGY_SAMPLEFORMAT_ANALYTIC) {
				for (i = 0; i < sbp->samples; i++) {
					segydata[2 * i] = (float)(((double)shortptr[2 * i]) / weight);
					segydata[2 * i + 1] = (float)(((double)shortptr[2 * i + 1]) / weight);
				}
			}

			/* else extract desired real trace from analytic data */
			else if (*sampleformat == MB_SEGY_SAMPLEFORMAT_TRACE) {
				for (i = 0; i < sbp->samples; i++) {
					segydata[i] = (float)(((double)shortptr[2 * i]) / weight);
				}
			}
		}
		else if (sbp->dataFormat == MBSYS_JSTAR_TRACEFORMAT_RAW) {
			*sampleformat = MB_SEGY_SAMPLEFORMAT_TRACE;
			for (i = 0; i < sbp->samples; i++) {
				segydata[i] = (float)(((double)ushortptr[i]) / weight);
			}
		}
		else if (sbp->dataFormat == MBSYS_JSTAR_TRACEFORMAT_REALANALYTIC) {
			*sampleformat = MB_SEGY_SAMPLEFORMAT_TRACE;
			for (i = 0; i < sbp->samples; i++) {
				segydata[i] = (float)(((double)ushortptr[i]) / weight);
			}
		}
		else if (sbp->dataFormat == MBSYS_JSTAR_TRACEFORMAT_PIXEL) {
			*sampleformat = MB_SEGY_SAMPLEFORMAT_TRACE;
			for (i = 0; i < sbp->samples; i++) {
				segydata[i] = (float)(((double)ushortptr[i]) / weight);
			}
		}

		/* done translating values */
	}

	/* deal with comment */
	else if (*kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       sampleformat:      %d\n", *sampleformat);
		fprintf(stderr, "dbg2       kind:              %d\n", *kind);
		fprintf(stderr, "dbg2       seq_num:           %d\n", mb_segytraceheader_ptr->seq_num);
		fprintf(stderr, "dbg2       seq_reel:          %d\n", mb_segytraceheader_ptr->seq_reel);
		fprintf(stderr, "dbg2       shot_num:          %d\n", mb_segytraceheader_ptr->shot_num);
		fprintf(stderr, "dbg2       shot_tr:           %d\n", mb_segytraceheader_ptr->shot_tr);
		fprintf(stderr, "dbg2       espn:              %d\n", mb_segytraceheader_ptr->espn);
		fprintf(stderr, "dbg2       rp_num:            %d\n", mb_segytraceheader_ptr->rp_num);
		fprintf(stderr, "dbg2       rp_tr:             %d\n", mb_segytraceheader_ptr->rp_tr);
		fprintf(stderr, "dbg2       trc_id:            %d\n", mb_segytraceheader_ptr->trc_id);
		fprintf(stderr, "dbg2       num_vstk:          %d\n", mb_segytraceheader_ptr->num_vstk);
		fprintf(stderr, "dbg2       cdp_fold:          %d\n", mb_segytraceheader_ptr->cdp_fold);
		fprintf(stderr, "dbg2       use:               %d\n", mb_segytraceheader_ptr->use);
		fprintf(stderr, "dbg2       range:             %d\n", mb_segytraceheader_ptr->range);
		fprintf(stderr, "dbg2       grp_elev:          %d\n", mb_segytraceheader_ptr->grp_elev);
		fprintf(stderr, "dbg2       src_elev:          %d\n", mb_segytraceheader_ptr->src_elev);
		fprintf(stderr, "dbg2       src_depth:         %d\n", mb_segytraceheader_ptr->src_depth);
		fprintf(stderr, "dbg2       grp_datum:         %d\n", mb_segytraceheader_ptr->grp_datum);
		fprintf(stderr, "dbg2       src_datum:         %d\n", mb_segytraceheader_ptr->src_datum);
		fprintf(stderr, "dbg2       src_wbd:           %d\n", mb_segytraceheader_ptr->src_wbd);
		fprintf(stderr, "dbg2       grp_wbd:           %d\n", mb_segytraceheader_ptr->grp_wbd);
		fprintf(stderr, "dbg2       elev_scalar:       %d\n", mb_segytraceheader_ptr->elev_scalar);
		fprintf(stderr, "dbg2       coord_scalar:      %d\n", mb_segytraceheader_ptr->coord_scalar);
		fprintf(stderr, "dbg2       src_long:          %d\n", mb_segytraceheader_ptr->src_long);
		fprintf(stderr, "dbg2       src_lat:           %d\n", mb_segytraceheader_ptr->src_lat);
		fprintf(stderr, "dbg2       grp_long:          %d\n", mb_segytraceheader_ptr->grp_long);
		fprintf(stderr, "dbg2       grp_lat:           %d\n", mb_segytraceheader_ptr->grp_lat);
		fprintf(stderr, "dbg2       coord_units:       %d\n", mb_segytraceheader_ptr->coord_units);
		fprintf(stderr, "dbg2       wvel:              %d\n", mb_segytraceheader_ptr->wvel);
		fprintf(stderr, "dbg2       sbvel:             %d\n", mb_segytraceheader_ptr->sbvel);
		fprintf(stderr, "dbg2       src_up_vel:        %d\n", mb_segytraceheader_ptr->src_up_vel);
		fprintf(stderr, "dbg2       grp_up_vel:        %d\n", mb_segytraceheader_ptr->grp_up_vel);
		fprintf(stderr, "dbg2       src_static:        %d\n", mb_segytraceheader_ptr->src_static);
		fprintf(stderr, "dbg2       grp_static:        %d\n", mb_segytraceheader_ptr->grp_static);
		fprintf(stderr, "dbg2       tot_static:        %d\n", mb_segytraceheader_ptr->tot_static);
		fprintf(stderr, "dbg2       laga:              %d\n", mb_segytraceheader_ptr->laga);
		fprintf(stderr, "dbg2       delay_mils:        %d\n", mb_segytraceheader_ptr->delay_mils);
		fprintf(stderr, "dbg2       smute_mils:        %d\n", mb_segytraceheader_ptr->smute_mils);
		fprintf(stderr, "dbg2       emute_mils:        %d\n", mb_segytraceheader_ptr->emute_mils);
		fprintf(stderr, "dbg2       nsamps:            %d\n", mb_segytraceheader_ptr->nsamps);
		fprintf(stderr, "dbg2       si_micros:         %d\n", mb_segytraceheader_ptr->si_micros);
		for (i = 0; i < 19; i++)
			fprintf(stderr, "dbg2       other_1[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_1[i]);
		fprintf(stderr, "dbg2       year:              %d\n", mb_segytraceheader_ptr->year);
		fprintf(stderr, "dbg2       day_of_yr:         %d\n", mb_segytraceheader_ptr->day_of_yr);
		fprintf(stderr, "dbg2       hour:              %d\n", mb_segytraceheader_ptr->hour);
		fprintf(stderr, "dbg2       min:               %d\n", mb_segytraceheader_ptr->min);
		fprintf(stderr, "dbg2       sec:               %d\n", mb_segytraceheader_ptr->sec);
		fprintf(stderr, "dbg2       mils:              %d\n", mb_segytraceheader_ptr->mils);
		fprintf(stderr, "dbg2       tr_weight:         %d\n", mb_segytraceheader_ptr->tr_weight);
		for (i = 0; i < 5; i++)
			fprintf(stderr, "dbg2       other_2[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_2[i]);
		fprintf(stderr, "dbg2       delay:             %f\n", mb_segytraceheader_ptr->delay);
		fprintf(stderr, "dbg2       smute_sec:         %f\n", mb_segytraceheader_ptr->smute_sec);
		fprintf(stderr, "dbg2       emute_sec:         %f\n", mb_segytraceheader_ptr->emute_sec);
		fprintf(stderr, "dbg2       si_secs:           %f\n", mb_segytraceheader_ptr->si_secs);
		fprintf(stderr, "dbg2       wbt_secs:          %f\n", mb_segytraceheader_ptr->wbt_secs);
		fprintf(stderr, "dbg2       end_of_rp:         %d\n", mb_segytraceheader_ptr->end_of_rp);
		fprintf(stderr, "dbg2       dummy1:            %f\n", mb_segytraceheader_ptr->dummy1);
		fprintf(stderr, "dbg2       dummy2:            %f\n", mb_segytraceheader_ptr->dummy2);
		fprintf(stderr, "dbg2       dummy3:            %f\n", mb_segytraceheader_ptr->dummy3);
		fprintf(stderr, "dbg2       dummy4:            %f\n", mb_segytraceheader_ptr->dummy4);
		fprintf(stderr, "dbg2       soundspeed:        %f\n", mb_segytraceheader_ptr->soundspeed);
		fprintf(stderr, "dbg2       distance:          %f\n", mb_segytraceheader_ptr->distance);
		fprintf(stderr, "dbg2       roll:              %f\n", mb_segytraceheader_ptr->roll);
		fprintf(stderr, "dbg2       pitch:             %f\n", mb_segytraceheader_ptr->pitch);
		fprintf(stderr, "dbg2       heading:           %f\n", mb_segytraceheader_ptr->heading);
		for (i = 0; i < mb_segytraceheader_ptr->nsamps; i++)
			fprintf(stderr, "dbg2       segydata[%d]:      %f\n", i, segydata[i]);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_insert_segy(int verbose, void *mbio_ptr, void *store_ptr, int kind, void *segyheader_ptr, float *segydata,
                            int *error) {
	char *function_name = "mbsys_jstar_insert_segy";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_channel_struct *sbp;
	struct mb_segytraceheader_struct *mb_segytraceheader_ptr;
	int sonardepth, waterdepth;
	int time_i[7];
	int time_j[5];
	float factor;
	float datamax;
	double weight;
	int data_size;
	short *shortptr;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:        %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:         %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:      %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       kind:           %d\n", kind);
		fprintf(stderr, "dbg2       segyheader_ptr: %p\n", (void *)segyheader_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get data kind */
	store->kind = kind;

	/* get segy traceheader */
	mb_segytraceheader_ptr = (struct mb_segytraceheader_struct *)segyheader_ptr;

	/* insert data to structure */
	if (store->kind == MB_DATA_SUBBOTTOM_SUBBOTTOM) {
		/* get channel */
		sbp = (struct mbsys_jstar_channel_struct *)&(store->sbp);
		shortptr = (short *)sbp->trace;

		/* extract the data */
		if (mb_segytraceheader_ptr->shot_num != 0)
			sbp->pingNum = mb_segytraceheader_ptr->shot_num;
		else if (mb_segytraceheader_ptr->seq_reel != 0)
			sbp->pingNum = mb_segytraceheader_ptr->seq_reel;
		else if (mb_segytraceheader_ptr->seq_num != 0)
			sbp->pingNum = mb_segytraceheader_ptr->seq_num;
		else if (mb_segytraceheader_ptr->rp_num != 0)
			sbp->pingNum = mb_segytraceheader_ptr->rp_num;
		else
			sbp->pingNum = 0;
		sbp->dataFormat = mb_segytraceheader_ptr->use;
		if (mb_segytraceheader_ptr->grp_elev != 0)
			sonardepth = -mb_segytraceheader_ptr->grp_elev;
		else if (mb_segytraceheader_ptr->src_elev != 0)
			sonardepth = -mb_segytraceheader_ptr->src_elev;
		else if (mb_segytraceheader_ptr->src_depth != 0)
			sonardepth = mb_segytraceheader_ptr->src_depth;
		else
			sonardepth = 0;
		if (mb_segytraceheader_ptr->elev_scalar < 0)
			factor = 1.0 / ((float)(-mb_segytraceheader_ptr->elev_scalar));
		else
			factor = (float)mb_segytraceheader_ptr->elev_scalar;
		if (mb_segytraceheader_ptr->src_wbd != 0)
			waterdepth = -mb_segytraceheader_ptr->grp_elev;
		else if (mb_segytraceheader_ptr->grp_wbd != 0)
			waterdepth = -mb_segytraceheader_ptr->src_elev;
		else
			waterdepth = 0;
		if (mb_segytraceheader_ptr->coord_scalar < 0)
			factor = 1.0 / ((float)(-mb_segytraceheader_ptr->coord_scalar)) / 3600.0;
		else
			factor = (float)mb_segytraceheader_ptr->coord_scalar / 3600.0;
		sbp->samples = mb_segytraceheader_ptr->nsamps;
		sbp->sampleInterval = 1000 * mb_segytraceheader_ptr->si_micros;
		time_j[0] = mb_segytraceheader_ptr->year;
		time_j[1] = mb_segytraceheader_ptr->day_of_yr;
		time_j[2] = 60 * mb_segytraceheader_ptr->hour + mb_segytraceheader_ptr->min;
		time_j[3] = mb_segytraceheader_ptr->sec;
		time_j[4] = 1000 * mb_segytraceheader_ptr->mils;
		mb_get_itime(verbose, time_j, time_i);
		sbp->year = time_j[0];
		sbp->day = time_j[1];
		sbp->second = 0.000001 * time_i[6] + time_i[5];
		sbp->hour = time_i[3];
		sbp->minute = time_i[4];
		sbp->millisecondsToday = 0.001 * time_i[6] + 1000 * (time_i[5] + 60.0 * (time_i[4] + 60.0 * time_i[3]));

		sbp->sonarDepth = 1000 * waterdepth;
		sbp->sonarDepth = 1000 * sonardepth;
		sbp->sonarAltitude = 1000 * (waterdepth - sonardepth);
		if (sbp->sonarAltitude < 0)
			sbp->sonarAltitude = 0;

		/* get max data value */
		datamax = 0.0;
		for (i = 0; i < mb_segytraceheader_ptr->nsamps; i++) {
			if (fabs(segydata[i]) > datamax)
				datamax = fabs(segydata[i]);
		}
		if (datamax > 0.0) {
			sbp->weightingFactor = (short)(log(datamax) / MB_LN_2) - 15;
		}
		else
			sbp->weightingFactor = 0;
		weight = pow(2.0, (double)sbp->weightingFactor);

		/* make sure enough memory is allocated for channel data */
		data_size = sizeof(short) * sbp->samples;
		if (sbp->trace_alloc < data_size) {
			status = mb_reallocd(verbose, __FILE__, __LINE__, data_size, (void **)&(sbp->trace), error);
			if (status == MB_SUCCESS) {
				sbp->trace_alloc = data_size;
			}
			else {
				sbp->trace_alloc = 0;
				sbp->samples = 0;
			}
		}

		/* copy over the data */
		if (sbp->trace_alloc >= data_size) {
			shortptr = (short *)sbp->trace;
			for (i = 0; i < sbp->samples; i++) {
				shortptr[i] = (short)(segydata[i] * weight);
			}
		}

		/* done translating values */
	}

	/* deal with comment */
	else if (kind == MB_DATA_COMMENT) {
		/* set status */
		*error = MB_ERROR_COMMENT;
		status = MB_FAILURE;
	}

	/* deal with other record type */
	else {
		/* set status */
		*error = MB_ERROR_OTHER;
		status = MB_FAILURE;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       seq_num:           %d\n", mb_segytraceheader_ptr->seq_num);
		fprintf(stderr, "dbg2       seq_reel:          %d\n", mb_segytraceheader_ptr->seq_reel);
		fprintf(stderr, "dbg2       shot_num:          %d\n", mb_segytraceheader_ptr->shot_num);
		fprintf(stderr, "dbg2       shot_tr:           %d\n", mb_segytraceheader_ptr->shot_tr);
		fprintf(stderr, "dbg2       espn:              %d\n", mb_segytraceheader_ptr->espn);
		fprintf(stderr, "dbg2       rp_num:            %d\n", mb_segytraceheader_ptr->rp_num);
		fprintf(stderr, "dbg2       rp_tr:             %d\n", mb_segytraceheader_ptr->rp_tr);
		fprintf(stderr, "dbg2       trc_id:            %d\n", mb_segytraceheader_ptr->trc_id);
		fprintf(stderr, "dbg2       num_vstk:          %d\n", mb_segytraceheader_ptr->num_vstk);
		fprintf(stderr, "dbg2       cdp_fold:          %d\n", mb_segytraceheader_ptr->cdp_fold);
		fprintf(stderr, "dbg2       use:               %d\n", mb_segytraceheader_ptr->use);
		fprintf(stderr, "dbg2       range:             %d\n", mb_segytraceheader_ptr->range);
		fprintf(stderr, "dbg2       grp_elev:          %d\n", mb_segytraceheader_ptr->grp_elev);
		fprintf(stderr, "dbg2       src_elev:          %d\n", mb_segytraceheader_ptr->src_elev);
		fprintf(stderr, "dbg2       src_depth:         %d\n", mb_segytraceheader_ptr->src_depth);
		fprintf(stderr, "dbg2       grp_datum:         %d\n", mb_segytraceheader_ptr->grp_datum);
		fprintf(stderr, "dbg2       src_datum:         %d\n", mb_segytraceheader_ptr->src_datum);
		fprintf(stderr, "dbg2       src_wbd:           %d\n", mb_segytraceheader_ptr->src_wbd);
		fprintf(stderr, "dbg2       grp_wbd:           %d\n", mb_segytraceheader_ptr->grp_wbd);
		fprintf(stderr, "dbg2       elev_scalar:       %d\n", mb_segytraceheader_ptr->elev_scalar);
		fprintf(stderr, "dbg2       coord_scalar:      %d\n", mb_segytraceheader_ptr->coord_scalar);
		fprintf(stderr, "dbg2       src_long:          %d\n", mb_segytraceheader_ptr->src_long);
		fprintf(stderr, "dbg2       src_lat:           %d\n", mb_segytraceheader_ptr->src_lat);
		fprintf(stderr, "dbg2       grp_long:          %d\n", mb_segytraceheader_ptr->grp_long);
		fprintf(stderr, "dbg2       grp_lat:           %d\n", mb_segytraceheader_ptr->grp_lat);
		fprintf(stderr, "dbg2       coord_units:       %d\n", mb_segytraceheader_ptr->coord_units);
		fprintf(stderr, "dbg2       wvel:              %d\n", mb_segytraceheader_ptr->wvel);
		fprintf(stderr, "dbg2       sbvel:             %d\n", mb_segytraceheader_ptr->sbvel);
		fprintf(stderr, "dbg2       src_up_vel:        %d\n", mb_segytraceheader_ptr->src_up_vel);
		fprintf(stderr, "dbg2       grp_up_vel:        %d\n", mb_segytraceheader_ptr->grp_up_vel);
		fprintf(stderr, "dbg2       src_static:        %d\n", mb_segytraceheader_ptr->src_static);
		fprintf(stderr, "dbg2       grp_static:        %d\n", mb_segytraceheader_ptr->grp_static);
		fprintf(stderr, "dbg2       tot_static:        %d\n", mb_segytraceheader_ptr->tot_static);
		fprintf(stderr, "dbg2       laga:              %d\n", mb_segytraceheader_ptr->laga);
		fprintf(stderr, "dbg2       delay_mils:        %d\n", mb_segytraceheader_ptr->delay_mils);
		fprintf(stderr, "dbg2       smute_mils:        %d\n", mb_segytraceheader_ptr->smute_mils);
		fprintf(stderr, "dbg2       emute_mils:        %d\n", mb_segytraceheader_ptr->emute_mils);
		fprintf(stderr, "dbg2       nsamps:            %d\n", mb_segytraceheader_ptr->nsamps);
		fprintf(stderr, "dbg2       si_micros:         %d\n", mb_segytraceheader_ptr->si_micros);
		for (i = 0; i < 19; i++)
			fprintf(stderr, "dbg2       other_1[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_1[i]);
		fprintf(stderr, "dbg2       year:              %d\n", mb_segytraceheader_ptr->year);
		fprintf(stderr, "dbg2       day_of_yr:         %d\n", mb_segytraceheader_ptr->day_of_yr);
		fprintf(stderr, "dbg2       hour:              %d\n", mb_segytraceheader_ptr->hour);
		fprintf(stderr, "dbg2       min:               %d\n", mb_segytraceheader_ptr->min);
		fprintf(stderr, "dbg2       sec:               %d\n", mb_segytraceheader_ptr->sec);
		fprintf(stderr, "dbg2       mils:              %d\n", mb_segytraceheader_ptr->mils);
		fprintf(stderr, "dbg2       tr_weight:         %d\n", mb_segytraceheader_ptr->tr_weight);
		for (i = 0; i < 5; i++)
			fprintf(stderr, "dbg2       other_2[%2d]:       %d\n", i, mb_segytraceheader_ptr->other_2[i]);
		fprintf(stderr, "dbg2       delay:             %f\n", mb_segytraceheader_ptr->delay);
		fprintf(stderr, "dbg2       smute_sec:         %f\n", mb_segytraceheader_ptr->smute_sec);
		fprintf(stderr, "dbg2       emute_sec:         %f\n", mb_segytraceheader_ptr->emute_sec);
		fprintf(stderr, "dbg2       si_secs:           %f\n", mb_segytraceheader_ptr->si_secs);
		fprintf(stderr, "dbg2       wbt_secs:          %f\n", mb_segytraceheader_ptr->wbt_secs);
		fprintf(stderr, "dbg2       end_of_rp:         %d\n", mb_segytraceheader_ptr->end_of_rp);
		fprintf(stderr, "dbg2       dummy1:            %f\n", mb_segytraceheader_ptr->dummy1);
		fprintf(stderr, "dbg2       dummy2:            %f\n", mb_segytraceheader_ptr->dummy2);
		fprintf(stderr, "dbg2       dummy3:            %f\n", mb_segytraceheader_ptr->dummy3);
		fprintf(stderr, "dbg2       dummy4:            %f\n", mb_segytraceheader_ptr->dummy4);
		fprintf(stderr, "dbg2       soundspeed:        %f\n", mb_segytraceheader_ptr->soundspeed);
		fprintf(stderr, "dbg2       distance:          %f\n", mb_segytraceheader_ptr->distance);
		fprintf(stderr, "dbg2       roll:              %f\n", mb_segytraceheader_ptr->roll);
		fprintf(stderr, "dbg2       pitch:             %f\n", mb_segytraceheader_ptr->pitch);
		fprintf(stderr, "dbg2       heading:           %f\n", mb_segytraceheader_ptr->heading);
		fprintf(stderr, "dbg2       error:             %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:            %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_ctd(int verbose, void *mbio_ptr, void *store_ptr, int *kind, int *nctd, double *time_d, double *conductivity,
                    double *temperature, double *depth, double *salinity, double *soundspeed, int *error) {
	char *function_name = "mbsys_jstar_ctd";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_pressure_struct *pressure;
	double g, x, p;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mb_ptr:     %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointer */
	store = (struct mbsys_jstar_struct *)store_ptr;

	/* get pressure record structure */
	pressure = (struct mbsys_jstar_pressure_struct *)&store->pressure;

	/* get data kind */
	*kind = store->kind;

	/* get the ctd data from the Edgetech pressure record */
	if (*kind == MB_DATA_CTD) {
		*nctd = 1;
		*time_d = pressure->seconds + 0.001 * pressure->milliseconds;
		*conductivity = 1000000.0 * pressure->conductivity;
		*temperature = 0.0;

		/* Convert pressure to depth using UNESCO equations in UNESCO Technical Paper Marine Science No. 44
		 *	http://www.seabird.com/application_notes/AN69.htm */
		p = 0.00068947 * pressure->pressure; /*convert pressure from 0.001 PSI to decibar */
		x = 0.0;                             /* sin(latitude) where latitude is assumed zero here */
		g = 9.780318 * (1.0 + (5.2788e-3 + 2.36e-5 * x) * x) + 1.092e-6 * p;

		*depth = ((((-1.82e-15 * p + 2.279e-10) * p - 2.2512e-5) * p + 9.72659) * p) / g;
		*salinity = 0.001 * pressure->salinity; /* convert from ppm to PSU */
		*soundspeed = 1000.0 * pressure->soundspeed;
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       kind:       %d\n", *kind);
	}
	if (verbose >= 2 && *error == MB_ERROR_NO_ERROR) {
		fprintf(stderr, "dbg2       nctd:          %d\n", *nctd);
		for (i = 0; i < *nctd; i++) {
			fprintf(stderr, "dbg2       time_d:        %f\n", time_d[i]);
			fprintf(stderr, "dbg2       conductivity:  %f\n", conductivity[i]);
			fprintf(stderr, "dbg2       temperature:   %f\n", temperature[i]);
			fprintf(stderr, "dbg2       depth:         %f\n", depth[i]);
			fprintf(stderr, "dbg2       salinity:      %f\n", salinity[i]);
			fprintf(stderr, "dbg2       soundspeed:    %f\n", soundspeed[i]);
		}
	}
	if (verbose >= 2) {
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
int mbsys_jstar_copyrecord(int verbose, void *mbio_ptr, void *store_ptr, void *copy_ptr, int *error) {
	char *function_name = "mbsys_jstar_copyrecord";
	int status = MB_SUCCESS;
	struct mb_io_struct *mb_io_ptr;
	struct mbsys_jstar_struct *store;
	struct mbsys_jstar_struct *copy;
	unsigned int sbp_trace_alloc;
	unsigned short *sbp_trace;
	unsigned int ssport_trace_alloc;
	unsigned short *ssport_trace;
	unsigned int ssstbd_trace_alloc;
	unsigned short *ssstbd_trace;
	int shortspersample;
	int trace_size;
	int i;

	/* print input debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> called\n", function_name);
		fprintf(stderr, "dbg2  Revision id: %s\n", svn_id);
		fprintf(stderr, "dbg2  Input arguments:\n");
		fprintf(stderr, "dbg2       verbose:    %d\n", verbose);
		fprintf(stderr, "dbg2       mbio_ptr:   %p\n", (void *)mbio_ptr);
		fprintf(stderr, "dbg2       store_ptr:  %p\n", (void *)store_ptr);
		fprintf(stderr, "dbg2       copy_ptr:   %p\n", (void *)copy_ptr);
	}

	/* get mbio descriptor */
	mb_io_ptr = (struct mb_io_struct *)mbio_ptr;

	/* get data structure pointers */
	store = (struct mbsys_jstar_struct *)store_ptr;
	copy = (struct mbsys_jstar_struct *)copy_ptr;

	/* save existing trace pointers in copy */
	sbp_trace_alloc = copy->sbp.trace_alloc;
	sbp_trace = copy->sbp.trace;
	ssport_trace_alloc = copy->ssport.trace_alloc;
	ssport_trace = copy->ssport.trace;
	ssstbd_trace_alloc = copy->ssstbd.trace_alloc;
	ssstbd_trace = copy->ssstbd.trace;

	/* copy the data */
	*copy = *store;

	/* restore the original trace pointers */
	copy->sbp.trace_alloc = sbp_trace_alloc;
	copy->sbp.trace = sbp_trace;
	copy->ssport.trace_alloc = ssport_trace_alloc;
	copy->ssport.trace = ssport_trace;
	copy->ssstbd.trace_alloc = ssstbd_trace_alloc;
	copy->ssstbd.trace = ssstbd_trace;

	/* allocate memory and copy each trace */

	/* allocate memory for the subbottom trace */
	if (copy->sbp.dataFormat == 1)
		shortspersample = 2;
	else
		shortspersample = 1;
	trace_size = shortspersample * copy->sbp.samples * sizeof(short);
	if (copy->sbp.trace_alloc < trace_size) {
		if ((status = mb_reallocd(verbose, __FILE__, __LINE__, trace_size, (void **)&(copy->sbp.trace), error)) == MB_SUCCESS) {
			copy->sbp.trace_alloc = trace_size;
		}
	}
	if (copy->sbp.trace_alloc >= trace_size) {
		for (i = 0; i < shortspersample * copy->sbp.samples; i++) {
			copy->sbp.trace[i] = store->sbp.trace[i];
		}
	}

	/* allocate memory for the port sidescan trace */
	if (copy->ssport.dataFormat == 1)
		shortspersample = 2;
	else
		shortspersample = 1;
	trace_size = shortspersample * copy->ssport.samples * sizeof(short);
	if (copy->ssport.trace_alloc < trace_size) {
		if ((status = mb_reallocd(verbose, __FILE__, __LINE__, trace_size, (void **)&(copy->ssport.trace), error)) ==
		    MB_SUCCESS) {
			copy->ssport.trace_alloc = trace_size;
		}
	}
	if (copy->ssport.trace_alloc >= trace_size) {
		for (i = 0; i < shortspersample * copy->ssport.samples; i++) {
			copy->ssport.trace[i] = store->ssport.trace[i];
		}
	}

	/* allocate memory for the starboard sidescan trace */
	if (copy->ssstbd.dataFormat == 1)
		shortspersample = 2;
	else
		shortspersample = 1;
	trace_size = shortspersample * copy->ssstbd.samples * sizeof(short);
	if (copy->ssstbd.trace_alloc < trace_size) {
		if ((status = mb_reallocd(verbose, __FILE__, __LINE__, trace_size, (void **)&(copy->ssstbd.trace), error)) ==
		    MB_SUCCESS) {
			copy->ssstbd.trace_alloc = trace_size;
		}
	}
	if (copy->ssstbd.trace_alloc >= trace_size) {
		for (i = 0; i < shortspersample * copy->ssstbd.samples; i++) {
			copy->ssstbd.trace[i] = store->ssstbd.trace[i];
		}
	}

	/* print output debug statements */
	if (verbose >= 2) {
		fprintf(stderr, "\ndbg2  MBIO function <%s> completed\n", function_name);
		fprintf(stderr, "dbg2  Return values:\n");
		fprintf(stderr, "dbg2       error:      %d\n", *error);
		fprintf(stderr, "dbg2  Return status:\n");
		fprintf(stderr, "dbg2       status:     %d\n", status);
	}

	return (status);
}
/*--------------------------------------------------------------------*/
