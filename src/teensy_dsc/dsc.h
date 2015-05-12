#ifndef __DSC_H__
#define __DSC_H__

char *HexEncoderValue(long value);
char *EncoderValue(long value, bool lead);
int32_t ngc_convert_encoder_value(int32_t encoder, long resolution);

#endif
