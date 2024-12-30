// SPDX-FileCopyrightText: 2023 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

package org.citron.citron_emu.fragments

import android.Manifest
import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.activity.OnBackPressedCallback
import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.NotificationManagerCompat
import androidx.core.content.ContextCompat
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.isVisible
import androidx.core.view.updatePadding
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.navigation.findNavController
import androidx.preference.PreferenceManager
import androidx.viewpager2.widget.ViewPager2.OnPageChangeCallback
import com.google.android.material.transition.MaterialFadeThrough
import kotlinx.coroutines.launch
import org.citron.citron_emu.NativeLibrary
import java.io.File
import org.citron.citron_emu.R
import org.citron.citron_emu.CitronApplication
import org.citron.citron_emu.adapters.SetupAdapter
import org.citron.citron_emu.databinding.FragmentSetupBinding
import org.citron.citron_emu.features.settings.model.Settings
import org.citron.citron_emu.model.HomeViewModel
import org.citron.citron_emu.model.SetupCallback
import org.citron.citron_emu.model.SetupPage
import org.citron.citron_emu.model.StepState
import org.citron.citron_emu.ui.main.MainActivity
import org.citron.citron_emu.utils.DirectoryInitialization
import org.citron.citron_emu.utils.NativeConfig
import org.citron.citron_emu.utils.ViewUtils
import org.citron.citron_emu.utils.ViewUtils.setVisible
import org.citron.citron_emu.utils.collect
import android.app.AlertDialog
import android.content.DialogInterface

class SetupFragment : Fragment() {
    private var _binding: FragmentSetupBinding? = null
    private val binding get() = _binding!!

    private val homeViewModel: HomeViewModel by activityViewModels()

    private lateinit var mainActivity: MainActivity

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentSetupBinding.inflate(layoutInflater)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        mainActivity = requireActivity() as MainActivity

        homeViewModel.setNavigationVisibility(visible = false, animated = false)

        val preferences = PreferenceManager.getDefaultSharedPreferences(CitronApplication.appContext)
        if (preferences.getBoolean(Settings.PREF_FIRST_APP_LAUNCH, true)) {
            showFirstLaunchDialog()
        } else {
            finishSetup()
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    private fun showFirstLaunchDialog() {
        AlertDialog.Builder(requireContext())
            .setTitle(R.string.first_launch_dialog_title)
            .setMessage(R.string.first_launch_dialog_message)
            .setPositiveButton(R.string.first_launch_dialog_ok) { _, _ ->
                finishSetup()
            }
            .setCancelable(false)
            .show()
    }

    private fun finishSetup() {
        PreferenceManager.getDefaultSharedPreferences(CitronApplication.appContext).edit()
            .putBoolean(Settings.PREF_FIRST_APP_LAUNCH, false)
            .apply()
        mainActivity.finishSetup(binding.root.findNavController())
    }
}
