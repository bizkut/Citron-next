// SPDX-FileCopyrightText: 2023 citron Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

plugins {
    // Core build plugins
    id("com.android.application") version "8.7.3" apply false
    id("com.android.library") version "8.7.3" apply false
    id("org.jetbrains.kotlin.android") version "1.9.22" apply false
    id("androidx.navigation.safeargs.kotlin") version "2.8.5" apply false
}

// Configure all subprojects
subprojects {
    afterEvaluate {
        // Common configurations for all subprojects
        configurations.all {
            resolutionStrategy {
                // Cache dynamic versions for 10 minutes
                cacheDynamicVersionsFor(10, "minutes")

                // Cache changing modules for 10 minutes
                cacheChangingModulesFor(10, "minutes")

                // Force specific versions if needed
                // force("some.dependency:version")
            }
        }
    }
}

tasks.register("clean").configure {
    delete(rootProject.buildDir)
}

// Gradle build scan - useful for debugging build issues
if (hasProperty("buildScan")) {
    extensions.findByName("buildScan")?.withGroovyBuilder {
        setProperty("termsOfServiceUrl", "https://gradle.com/terms-of-service")
        setProperty("termsOfServiceAgree", "yes")
    }
}
