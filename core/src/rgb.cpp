unsigned int GLT_RGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (((b >> 3) & 0x0000001F) | (((g >> 2) << 5) & 0x00007e0) | (((r >> 3) << 11) & 0xf800));
}

unsigned char GLT_RGB_R(unsigned int rgb)
{
	return ((rgb >> 8 )&0xF8);
}

unsigned char GLT_RGB_G(unsigned int rgb)
{
	return ((rgb >> 3 )&0xFC);
}

unsigned char GLT_RGB_B(unsigned int rgb)
{
	return ((rgb << 3)&0xF8);
}
