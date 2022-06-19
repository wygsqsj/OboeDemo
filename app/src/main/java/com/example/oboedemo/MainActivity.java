package com.example.oboedemo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.example.oboedemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private String mp3path;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        requestRecordPermission();
        mp3path = FileUtil.copyAssFileToSD(this, "渡口.mp3");
    }

    private void requestRecordPermission() {
        ActivityCompat.requestPermissions(
                this,
                new String[]{Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                123);
    }


    public void start(View view) {
        startRecord(getExternalFilesDir("") + "/audio.pcm");
    }

    public void stop(View view) {
        stopRecord();
    }

    public void startPlay(View view) {
        startPlay(mp3path);
    }

    public void stopPlay(View view) {
        stopPlay();
    }


    public native void startRecord(String pcmPath);

    public native void stopRecord();

    public native void startPlay(String mp3Path);

    public native void stopPlay();
}