/*
 *  Copyright (C) 2012 Bastien Nocera <hadess@hadess.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 *
 *
 * The Xplayer project hereby grant permission for non-gpl compatible GStreamer
 * plugins to be used and distributed together with GStreamer and Xplayer. This
 * permission are above and beyond the permissions granted by the GPL license
 * Xplayer is covered by.
 *
 * Monday 7th February 2005: Christian Schaller: Add exception clause.
 * See license_change file for details.
 *
 */


#include "config.h"

#include <glib-object.h>

#include "xplayer-plugin.h"
#include "xplayer.h"

#define XPLAYER_TYPE_APPLE_TRAILERS_PLUGIN	(xplayer_apple_trailers_plugin_get_type ())
#define XPLAYER_APPLE_TRAILERS_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), XPLAYER_TYPE_APPLE_TRAILERS_PLUGIN, XplayerAppleTrailersPlugin))

typedef struct {
	guint signal_id;
	XplayerObject *xplayer;
} XplayerAppleTrailersPluginPrivate;

XPLAYER_PLUGIN_REGISTER(XPLAYER_TYPE_APPLE_TRAILERS_PLUGIN, XplayerAppleTrailersPlugin, xplayer_apple_trailers_plugin)

static char *
get_user_agent_cb (XplayerObject *xplayer,
		   const char  *mrl)
{
	if (g_str_has_prefix (mrl, "http://movies.apple.com") ||
	    g_str_has_prefix (mrl, "http://trailers.apple.com"))
		return g_strdup ("Quicktime/7.2.0");
	return NULL;
}

static void
impl_activate (PeasActivatable *plugin)
{
	XplayerAppleTrailersPlugin *pi = XPLAYER_APPLE_TRAILERS_PLUGIN (plugin);

	pi->priv->xplayer = g_object_ref (g_object_get_data (G_OBJECT (plugin), "object"));
	pi->priv->signal_id = g_signal_connect (G_OBJECT (pi->priv->xplayer), "get-user-agent",
						G_CALLBACK (get_user_agent_cb), NULL);
}

static void
impl_deactivate (PeasActivatable *plugin)
{
	XplayerAppleTrailersPlugin *pi = XPLAYER_APPLE_TRAILERS_PLUGIN (plugin);

	if (pi->priv->signal_id) {
		g_signal_handler_disconnect (pi->priv->xplayer, pi->priv->signal_id);
		pi->priv->signal_id = 0;
	}

	if (pi->priv->xplayer) {
		g_object_unref (pi->priv->xplayer);
		pi->priv->xplayer = NULL;
	}
}
