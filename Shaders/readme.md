## Shader code to run via Material Graph "Custom" node

### Texture Shader

#### Invert Shader

```
return 1 - Texture2DSample(tex, texSampler, uv);
```

#### Common Convolution Filter Code

```
// input
// uv = TexCoord output
// tex = Texture Object output
// output
// Material: Base Color

int kernelSize = XXX;
float kernel[kernelSize] = {
  // the kernel, see below
};
int radius = XXX; // kernel radius, aka floor(sqrt(kernelSize) / 2)
int divisor = XXX; // aka sum(all values in kernel), only needed if != 0
float factor = 1; // can be set to intensify the result

int width = 1000; // of the texture -> should be and input value
int height = 1000;
float2 delta = float2(1.0f / width, 1.0f / height);

float4 value = float4(0.0f, 0.0f, 0.0f, 0.0f);
int x = -radius;
int y = radius;
for (int i = 0; i < kernelSize; i++) {
  value += Texture2DSample(tex, texSampler, uv + float2(x, y) * delta) * kernel[i] * factor;
  x++;
  if (x > radius) x = -radius;     
  y--;
  if (y < -radius) y = radius;
}

return value / divisor;
```

#### Motion Blur Kernel

```
int kernelSize = 81;
float kernel[kernelSize] = {
  1, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1,
};
int radius = 4;
int divisor = 9;
```

#### Laplace Edge Detection Kernels

```
int kernelSize = 9;
float kernel[kernelSize] = {
  0, -1, 0,
  -1, 4, -1,
  0, -1, 0
};
int radius = 1;
float factor = 3; // lightens up the otherwise quite dark result
```

```
int kernelSize = 49;
float kernel[kernelSize] = {
   0, 0, 0,-1, 0, 0, 0,
   0, 0, 0,-2, 0, 0, 0,
   0, 0, 0,-4, 0, 0, 0,
  -1,-2,-4,28,-4,-2,-1,
   0, 0, 0,-4, 0 ,0, 0,
   0, 0, 0,-2, 0 ,0, 0,
   0, 0, 0,-1, 0, 0, 0
};
int radius = 3;
float factor = 3;
```

```
int kernelSize = 9;
float kernel[kernelSize] = {
  -1, -1, -1,
  -1,  8, -1,
  -1, -1, -1
};
int radius = 1;
float factor = 3;
```

#### Scobel Edge Detection Kernel

```
int kernelSize = 9;
float kernelX[kernelSize] = {
  -1, 0, 1,
  -2, 0, 2,
  -1, 0, 1
};
float kernelY[kernelSize] = {
  1, 2, 1,
  0, 0, 0,
  -1, -2, -1
};
int radius = 1;
float factor = 3;

// and replace kernel[i] with float2(kernelX[i], kernelY[i])
```

### Post Process Shader

```
// input
// uv = `GetSceneTextureUV(Parameters);`
// tex = SceneTexture:PostProcessInput0 output Color
// res = SceneTexture:PostProcessInput0 output Size
// output
// Post Process Material: Emissive Color
```

- take any shader above and replace:
  - Texture2DSample(tex, texSampler, uv + float2(x, y) * delta) with
  - SceneTextureLookup(uv + float2(x, y) * delta, 14, false)
    - 14 is the index of PostProcessInput0 in the SceneTexture dropdown
	- false: no mipmap

