#ifndef __BT_FLAC 
#define __BT_FLAC

bool decoder_start(const char *flac_path);

void decoder_stop(void);
int32_t flac_a2d_data_cb(uint8_t *data, int32_t len);
int decode_flac_bt(const char *filename);
#endif // __BT_FLAC
