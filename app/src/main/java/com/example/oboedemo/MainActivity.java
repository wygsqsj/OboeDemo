package com.example.oboedemo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
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
                new String[]{Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE
                        , Manifest.permission.READ_EXTERNAL_STORAGE},
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

    //重采样
    public void startReSampling(View view) {
        @SuppressLint("WrongViewCast")
        EditText editText = findViewById(R.id.editReSampling);
        String edit = editText.getText().toString();
        int resampleQualityType = 2;//默认fast
        if (!TextUtils.isEmpty(edit)) {
            resampleQualityType = Integer.valueOf(edit);
        }
        startReSampling(resampleQualityType);
    }

    //原版重采样
    public void startOldReSampling(View view) {
        startOldReSampling();
    }


    public native void startRecord(String pcmPath);

    public native void stopRecord();

    public native void startPlay(String mp3Path);

    public native void stopPlay();

    public native void startReSampling(int resampleQualityType);

    public native void startOldReSampling();

}