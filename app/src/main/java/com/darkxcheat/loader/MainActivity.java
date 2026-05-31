package com.darkxcheat.loader;

import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;

import com.darkxcheat.loader.databinding.ActivityMainBinding;
import org.lsposed.lsparanoid.Obfuscate;

import top.niunaijun.blackbox.BlackBoxCore;

@Obfuscate
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MagicLoader";
    private static final int REQUEST_PERMISSIONS = 100;

    private ActivityMainBinding binding;
    
    private String supportedPackage;
    private String supportedVersion;
    private String updateUrl;
    private String telegramUrl;
    private boolean permissionsGranted = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setSupportActionBar(binding.toolbar);
        
        loadNativeConfig();
        
        if (hasRequiredPermissions()) {
            binding.grantPermBtn.hide();
            binding.grantPermBtn.setVisibility(View.GONE);
            permissionsGranted = true;
            checkGameStatus();
        }
        
        setupClickListeners();
    }

    private void setupClickListeners() {
        binding.grantPermBtn.setOnClickListener(v -> requestPermissions());
        binding.startBtn.setOnClickListener(v -> launchGame());
        binding.updateBtn.setOnClickListener(v -> openUpdateUrl());
        binding.telegramBtn.setOnClickListener(v -> openUrl(telegramUrl));
    }

    private void loadNativeConfig() {
        try {
            supportedPackage = Main.getSupportedPackage();
            supportedVersion = Main.getSupportedVersion();
            updateUrl = Main.getUpdateUrl();
            telegramUrl = Main.getTelegramUrl();
            
            Log.i(TAG, "Config: pkg=" + supportedPackage + " v=" + supportedVersion);
            
            loadGameInfo();
        } catch (Throwable e) {
            Log.e(TAG, "Failed to load config", e);
            loadGameInfo();
        }
    }

    private void loadGameInfo() {
        try {
            PackageManager pm = getPackageManager();
            ApplicationInfo appInfo = pm.getApplicationInfo(supportedPackage, 0);
            String appLabel = pm.getApplicationLabel(appInfo).toString();
            Drawable icon = pm.getApplicationIcon(appInfo);
            
            binding.gameName.setText(appLabel);
            binding.gameVersion.setText("v" + pm.getPackageInfo(supportedPackage, 0).versionName);
            binding.gameIcon.setImageDrawable(icon);
        } catch (Exception e) {
            binding.gameName.setText(R.string.app_name);
            binding.gameVersion.setText("v" + supportedVersion);
            Log.w(TAG, "Could not load game info", e);
        }
    }

    private void requestPermissions() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q) {
            if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    REQUEST_PERMISSIONS);
                return;
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.POST_NOTIFICATIONS)
                != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.POST_NOTIFICATIONS},
                    REQUEST_PERMISSIONS);
                return;
            }
        }
        
        onPermissionsGranted();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                            @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        
        boolean granted = true;
        for (int result : grantResults) {
            if (result != PackageManager.PERMISSION_GRANTED) {
                granted = false;
                break;
            }
        }
        
        if (granted) {
            onPermissionsGranted();
        } else {
            Toast.makeText(this, "Permissions required", Toast.LENGTH_SHORT).show();
        }
    }

    private void onPermissionsGranted() {
        permissionsGranted = true;
        binding.grantPermBtn.hide();
        binding.grantPermBtn.setVisibility(View.GONE);
        binding.statusText.setText(R.string.permissions_granted);
        updateStatusIcon(true);
        checkGameStatus();
    }
    
    private void updateStatusIcon(boolean success) {
        if (success) {
            binding.statusIcon.setImageResource(R.drawable.ic_check_circle);
            binding.statusIcon.setImageTintList(ContextCompat.getColorStateList(this, R.color.success));
        } else {
            binding.statusIcon.setImageResource(R.drawable.ic_error);
            binding.statusIcon.setImageTintList(ContextCompat.getColorStateList(this, R.color.error));
        }
    }
    
    private boolean hasRequiredPermissions() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q) {
            return ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            return ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.POST_NOTIFICATIONS) == PackageManager.PERMISSION_GRANTED;
        }
        return true;
    }

    private void checkGameStatus() {
        binding.statusText.setText(R.string.status_checking);
        updateStatusIcon(false);
        
        try {
            PackageManager pm = getPackageManager();
            pm.getApplicationInfo(supportedPackage, 0);
            String installedVersion = pm.getPackageInfo(supportedPackage, 0).versionName;
            
            binding.gameVersion.setText("v" + installedVersion);
            Log.i(TAG, "Game installed: " + installedVersion);
            
            boolean match = Main.checkVersionMatch(installedVersion);
            
            if (match) {
                binding.statusText.setText(R.string.status_ready);
                updateStatusIcon(true);
                binding.startBtn.setEnabled(true);
                binding.startBtn.setAlpha(1f);
            } else {
                binding.statusText.setText(R.string.version_mismatch);
                updateStatusIcon(false);
                binding.updateBtn.setVisibility(View.VISIBLE);
                
                new MaterialAlertDialogBuilder(this)
                    .setTitle("Version Mismatch")
                    .setMessage("Your version (" + installedVersion + ") != supported (" + supportedVersion + ")")
                    .setPositiveButton("Update", (d, w) -> openUpdateUrl())
                    .setNegativeButton("Cancel", null)
                    .show();
            }
        } catch (PackageManager.NameNotFoundException e) {
            binding.statusText.setText(R.string.status_not_installed);
            updateStatusIcon(false);
            binding.updateBtn.setVisibility(View.VISIBLE);
            Log.w(TAG, "Game not installed");
        } catch (Exception e) {
            Log.e(TAG, "Error checking game", e);
            binding.statusText.setText(R.string.launch_failed);
            updateStatusIcon(false);
        }
    }

    private void launchGame() {
        if (!permissionsGranted) {
            Toast.makeText(this, "Grant permissions first", Toast.LENGTH_SHORT).show();
            return;
        }
        
        try {
            binding.statusText.setText(R.string.launching);
            updateStatusIcon(false);
            binding.startBtn.setEnabled(false);
            
            // Call native Start() method before launching
            //Main.start();
            
            BlackBoxCore.get().installPackageAsUser(supportedPackage, 0);
            BlackBoxCore.get().launchApk(supportedPackage, 0);
            
            new android.os.Handler(android.os.Looper.getMainLooper())
                .postDelayed(this::finish, 500);
                
        } catch (Exception e) {
            Log.e(TAG, "Failed to launch", e);
            Toast.makeText(this, R.string.launch_failed, Toast.LENGTH_SHORT).show();
            binding.statusText.setText(R.string.launch_failed);
            updateStatusIcon(false);
            binding.startBtn.setEnabled(true);
        }
    }

    private void openUpdateUrl() {
        openUrl(updateUrl);
    }

    private void openUrl(String url) {
        if (url == null || url.isEmpty() || "YourLinkHere".equals(url)) {
            Toast.makeText(this, "No link available", Toast.LENGTH_SHORT).show();
            return;
        }
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
            startActivity(intent);
        } catch (Exception e) {
            Log.e(TAG, "Failed to open URL: " + url, e);
            Toast.makeText(this, "Cannot open link", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (item.getItemId() == R.id.action_about) {
            new MaterialAlertDialogBuilder(this)
                .setTitle(R.string.app_name)
                .setMessage(R.string.about_text)
                .setPositiveButton("OK", null)
                .show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}