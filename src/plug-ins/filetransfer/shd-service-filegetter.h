/**
 * The Shadow Simulator
 *
 * Copyright (c) 2010-2011 Rob Jansen <jansen@cs.umn.edu>
 * Copyright (c) 2006-2009 Tyson Malchow <tyson.malchow@gmail.com>
 *
 * This file is part of Shadow.
 *
 * Shadow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shadow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shadow.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHD_SERVICE_FILEGETTER_H_
#define SHD_SERVICE_FILEGETTER_H_

#include <glib.h>
#include <stddef.h>
#include <time.h>

#include "shd-filetransfer.h"
#include "shd-cdf.h"
#include "orderedlist.h"

enum service_filegetter_loglevel {
	SFG_CRITICAL, SFG_WARNING, SFG_NOTICE, SFG_INFO, SFG_DEBUG
};

enum service_filegetter_state {
	SFG_NONE, SFG_THINKING, SFG_DOWNLOADING, SFG_DONE
};

enum service_filegetter_type {
	SFG_SINGLE, SFG_DOUBLE, SFG_MULTI
};

typedef void (*service_filegetter_log_cb)(enum service_filegetter_loglevel level, const gchar* message);
typedef void (*service_filegetter_sleep_cb)(gpointer sfg, guint seconds);
typedef in_addr_t (*service_filegetter_hostbyname_cb)(const gchar* hostname);

typedef struct service_filegetter_server_args_s {
	gchar* host;
	gchar* port;
} service_filegetter_server_args_t, *service_filegetter_server_args_tp;

typedef struct service_filegetter_single_args_s {
	service_filegetter_server_args_t http_server;
	service_filegetter_server_args_t socks_proxy;
	service_filegetter_log_cb log_cb;
	service_filegetter_hostbyname_cb hostbyname_cb;
	gchar* num_downloads;
	gchar* filepath;
} service_filegetter_single_args_t, *service_filegetter_single_args_tp;

typedef struct service_filegetter_double_args_s {
	service_filegetter_server_args_t http_server;
	service_filegetter_server_args_t socks_proxy;
	service_filegetter_log_cb log_cb;
	service_filegetter_hostbyname_cb hostbyname_cb;
	service_filegetter_sleep_cb sleep_cb;
	gchar* pausetime_seconds;
	gchar* filepath1;
	gchar* filepath2;
	gchar* filepath3;
} service_filegetter_double_args_t, *service_filegetter_double_args_tp;

typedef struct service_filegetter_multi_args_s {
	gchar* server_specification_filepath;
	gchar* thinktimes_cdf_filepath;
	gchar* runtime_seconds;
	service_filegetter_server_args_t socks_proxy;
	service_filegetter_hostbyname_cb hostbyname_cb;
	service_filegetter_sleep_cb sleep_cb;
	service_filegetter_log_cb log_cb;
} service_filegetter_multi_args_t, *service_filegetter_multi_args_tp;

typedef struct service_filegetter_download_s {
	filegetter_filespec_t fspec;
	filegetter_serverspec_t sspec;
} service_filegetter_download_t, *service_filegetter_download_tp;

typedef struct service_filegetter_s {
	enum service_filegetter_state state;
	enum service_filegetter_type type;
	filegetter_t fg;
	orderedlist_tp downloads;
	service_filegetter_download_tp current_download;
	service_filegetter_download_tp download1;
	service_filegetter_download_tp download2;
	service_filegetter_download_tp download3;
	service_filegetter_hostbyname_cb hostbyname_cb;
	service_filegetter_sleep_cb sleep_cb;
	service_filegetter_log_cb log_cb;
	cdf_tp think_times;
	gint pausetime_seconds;
	struct timespec wakeup;
	struct timespec expire;
	gchar log_buffer[1024];
	gint downloads_requested;
	gint downloads_completed;
} service_filegetter_t, *service_filegetter_tp;

enum filegetter_code service_filegetter_start_single(service_filegetter_tp sfg, service_filegetter_single_args_tp args, gint* sockd_out);
enum filegetter_code service_filegetter_start_double(service_filegetter_tp sfg, service_filegetter_double_args_tp args, gint* sockd_out);
enum filegetter_code service_filegetter_start_multi(service_filegetter_tp sfg, service_filegetter_multi_args_tp args, gint* sockd_out);
enum filegetter_code service_filegetter_activate(service_filegetter_tp sfg, gint sockd);
enum filegetter_code service_filegetter_stop(service_filegetter_tp sfg);

#endif /* SHD_SERVICE_FILEGETTER_H_ */