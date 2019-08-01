package ar.com.mbernardi.sdl_fungeoid;

import android.content.Intent;
import android.net.Uri;
import java.io.File;

import ar.com.mbernardi.sdl_fungeoid.FileActivity;

public class OpenFileActivity extends FileActivity {

    @Override
    int getLayout() {
        return R.layout.activity_openfile;
    }

    @Override
    public void itemClicked(File file) {
        Intent data = new Intent();
        data.setData(Uri.parse(file.getAbsolutePath()));
        setResult(RESULT_OK, data);
        finish();
    }
}
