package ar.com.mbernardi.sdl_fungeoid;

import android.widget.Toast;
import android.net.Uri;
import android.content.Intent;
import android.util.Log;
import android.app.Activity;

import org.libsdl.app.SDLActivity;

public class SdlFungeoid extends SDLActivity {

    private static final int OPEN_REQUEST_CODE = 1;
    private static final int EXTERNAL_OPEN_REQUEST_CODE = 2;
    private static final int EXTERNAL_CREATE_REQUEST_CODE = 3;

    public void test() {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), "hola", Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void open_file() {
        Intent intent = new Intent(this, OpenFileActivity.class);
        startActivityForResult(intent, OPEN_REQUEST_CODE);
    }

    public void open_external_file() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("text/*");

        startActivityForResult(intent, EXTERNAL_OPEN_REQUEST_CODE);
    }

    public void create_external_file(String filename) {
        Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_TITLE, filename);

        startActivityForResult(intent, EXTERNAL_CREATE_REQUEST_CODE);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode,
            Intent resultData) {

        if (requestCode == OPEN_REQUEST_CODE && resultCode == Activity.RESULT_OK) {
            Uri uri = null;
            if (resultData != null) {
                uri = resultData.getData();
                Log.i("qwerty", "Uri: " + uri.toString());
                Toast.makeText(getApplicationContext(), uri.toString(), Toast.LENGTH_SHORT).show();
                onNativeDropFile(uri.toString());
            }
        }

        if (requestCode == EXTERNAL_OPEN_REQUEST_CODE && resultCode == Activity.RESULT_OK) {
            // The document selected by the user won't be returned in the intent.
            // Instead, a URI to that document will be contained in the return intent
            // provided to this method as a parameter.
            // Pull that URI using resultData.getData().
            Uri uri = null;
            if (resultData != null) {
                uri = resultData.getData();
                Log.i("qwerty", "Uri: " + uri.toString());
            }
        }
    }
}
