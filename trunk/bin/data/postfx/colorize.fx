texture framebuffer
vec3 color
float intensity

effect
{
	vec4 pixel = framebuffer(_in);
	float gray = pixel.r * 0.39 + pixel.g * 0.50 + pixel.b * 0.11;

	_out = vec4(gray * color, 1.0) * (1.0 - intensity) + pixel * intensity;
}
