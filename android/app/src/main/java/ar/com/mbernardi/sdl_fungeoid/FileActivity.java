package ar.com.mbernardi.sdl_fungeoid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.LinearLayoutManager;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.net.Uri;
import java.util.ArrayList;
import java.io.File;

import ar.com.mbernardi.sdl_fungeoid.FilenameAdapter;

public abstract class FileActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private RecyclerView.Adapter openFileAdapter;
    private RecyclerView.LayoutManager layoutManager;
    private ArrayList<File> fileList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getLayout());

        // So the back button on the ActionBar works
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        // Get list of files

        fileList = getFileList();

        // RecyclerView that holds list of files

        recyclerView = (RecyclerView) findViewById(R.id.files_recycler);

        // Improves performance as the list is fixed
        recyclerView.setHasFixedSize(true);

        layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);

        openFileAdapter = new FilenameAdapter(this, fileList);
        recyclerView.setAdapter(openFileAdapter);
    }

    ArrayList<File> getFileList() {
        if (android.os.Environment.getExternalStorageState()
                .equals(android.os.Environment.MEDIA_MOUNTED)) {

            File directory = getExternalFilesDir(null);
            File fileArray[] = directory.listFiles();
            ArrayList<File> fileList = new ArrayList<File>();

            if (fileArray != null) {
                for (int i = 0; i < fileArray.length; i++) {
                    if (!fileArray[i].isDirectory()) {
                        fileList.add(fileArray[i]);
                    }
                }
            }
            return fileList;
        }
        else
        {
            return null;
        }
    }

    // So the back button on the ActionBar works
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                finish();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * Called when a file is selected
     */
    public abstract void itemClicked(File file);

    /**
     * Get the layout to use. e.g.: R.layout.activity_openfile
     */
    abstract int getLayout();
}
