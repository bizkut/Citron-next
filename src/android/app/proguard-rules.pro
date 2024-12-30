# SPDX-FileCopyrightText: 2023 citron Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

### Basic Configuration ###
# Keep source file names and line numbers for better crash reports
-keepattributes SourceFile,LineNumberTable
-dontobfuscate
-dontoptimize
-verbose

### Kotlin ###
# Keep Kotlin metadata
-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod
-keep class kotlin.** { *; }
-keep class kotlinx.** { *; }
-keepclassmembers class **$WhenMappings {
    <fields>;
}

### Ini4j ###
# Prevents crashing when using Wini
-keep class org.ini4j.** { *; }
-keep class org.ini4j.spi.** { *; }

### Serialization ###
# Keep Serializable classes & fields
-keepclassmembers class * implements java.io.Serializable {
    private static final java.io.ObjectStreamField[] serialPersistentFields;
    private void writeObject(java.io.ObjectOutputStream);
    private void readObject(java.io.ObjectInputStream);
    java.lang.Object writeReplace();
    java.lang.Object readResolve();
}

### Security Libraries ###
# Suppress warnings for various security providers
-dontwarn org.bouncycastle.**
-dontwarn org.conscrypt.**
-dontwarn org.openjsse.**
-dontwarn java.beans.**

### Reflection ###
# Keep classes that are accessed via reflection
-keepclassmembers class * {
    @android.webkit.JavascriptInterface <methods>;
}

### Android ###
# Keep custom views
-keep public class * extends android.view.View {
    public <init>(android.content.Context);
    public <init>(android.content.Context, android.util.AttributeSet);
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

# Keep custom application class
-keep public class * extends android.app.Application

# Keep native methods
-keepclasseswithmembernames class * {
    native <methods>;
}
