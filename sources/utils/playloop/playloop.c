//this example is taken from: https://github.com/w-flo/gstreamer-gapless-test/blob/master/test-loop.c

#include <assert.h>
#include <gst/gst.h>
#include <glib.h>
#include <string.h>
char uri_string[1024];
struct NextStreamData {
	GstElement *pipeline;
	int count;
	int uri_count;
	char **uris;
};

static void prepare_next_stream(GstElement *obj, gpointer userdata) {
	struct NextStreamData *data = (struct NextStreamData*) userdata;
	//const char* next_uri = data->uris[MIN(data->count+1, data->uri_count)];
	//g_print("about-to-finish %4d; setting next to %s\n",
	//	data->count, next_uri);
	gst_element_set_state(data->pipeline,GST_STATE_READY);	
	g_object_set(G_OBJECT(data->pipeline), "uri", uri_string, NULL);
	//data->count++;
	gst_element_set_state(data->pipeline,GST_STATE_PLAYING);	
}

int main (int argc, char *argv[]) {
	GMainLoop *loop;
	GstElement *pipeline;

	/* Initialisation */
	gst_init (&argc, &argv);

	loop = g_main_loop_new (NULL, FALSE);

	/* Check input arguments */
	if (argc < 2) {
		g_printerr ("Usage: %s <list of space-delimited filenames or URLs>\n",
			argv[0]);
		return -1;
	}

	/* Create playbin. Different names in different versions  */
	pipeline = gst_element_factory_make("playbin", "playbin");
	if(pipeline == NULL)
		return -1;//NULL;

	/* Register about-to-finish callback to re-set the URI */
	struct NextStreamData replay_data;
	replay_data.pipeline = pipeline;
	replay_data.count = 0;
	replay_data.uris = &(argv[1]);
	replay_data.uri_count = argc-2;
	g_signal_connect(pipeline, "about-to-finish",
		G_CALLBACK(prepare_next_stream), &replay_data);

	/* Set initial URI */
	g_object_set (G_OBJECT (pipeline), "uri", argv[1], NULL);
	strcpy(uri_string,argv[1]);
	
	g_print ("Now playing: %s\n", argv[1]);
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	/* Iterate */
	g_print ("Running...\n");
	g_main_loop_run (loop);

	/* since we loop endlessly, the follwing is never reached */

	/* Out of the main loop, clean up nicely */
	g_print ("Returned, stopping playback\n");
	gst_element_set_state (pipeline, GST_STATE_NULL);

	g_print ("Deleting pipeline\n");
	gst_object_unref (GST_OBJECT (pipeline));
	g_main_loop_unref (loop);

	return 0;
}
