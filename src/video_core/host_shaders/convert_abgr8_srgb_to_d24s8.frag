// SPDX-FileCopyrightText: Copyright 2024 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#version 450
#extension GL_ARB_shader_stencil_export : require

layout(binding = 0) uniform sampler2D color_texture;

// Optimized sRGB to linear conversion
highp float srgbToLinear(highp float c) {
    return c <= 0.04045 ? c * (1.0 / 12.92) : pow((c + 0.055) * (1.0 / 1.055), 2.4);
}

void main() {
    // Direct texel fetch for better performance
    vec4 srgb = texelFetch(color_texture, ivec2(gl_FragCoord.xy), 0);

    // Convert to linear space and compute luminance in one step
    highp float luminance = dot(
        vec3(srgbToLinear(srgb.r), srgbToLinear(srgb.g), srgbToLinear(srgb.b)),
        vec3(0.2126, 0.7152, 0.0722)
    );

    // Optimize bit operations
    const float depth_max = 16777215.0; // 2^24 - 1
    highp uint depth_val = uint(luminance * depth_max);
    highp uint stencil_val = uint(srgb.a * 255.0);

    // Combine depth and stencil with minimal operations
    gl_FragDepth = float(depth_val) / depth_max;
    gl_FragStencilRefARB = int(stencil_val);
}
