package ar.com.mbernardi.sdl_fungeoid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.DividerItemDecoration;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.TextView;
import android.widget.FrameLayout;
import android.widget.Button;
import android.net.Uri;
import java.util.ArrayList;
import java.io.File;

import ar.com.mbernardi.sdl_fungeoid.OpenFileActivity;

public class OpenFileActivity extends AppCompatActivity implements ClickListener {

    private RecyclerView recyclerView;
    private RecyclerView.Adapter openFileAdapter;
    private RecyclerView.LayoutManager layoutManager;
    private ArrayList<File> fileList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_openfile);

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

        // Set up button

        final Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener()  {
            public void onClick(View v) {
                Intent data = new Intent();
                String text = "Result...";
                data.setData(Uri.parse(text));
                setResult(RESULT_OK, data);
                finish();
            }
        });
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

    public void itemClicked(File file) {
        Intent data = new Intent();
        data.setData(Uri.parse(file.getAbsolutePath()));
        setResult(RESULT_OK, data);
        finish();
    }
}

/**
 * Receives events from clicks on the RecyclerView
 *
 * The Activity extends this
 */
interface ClickListener {
    public void itemClicked(File file);
}

class FilenameAdapter extends RecyclerView.Adapter<FilenameAdapter.FilenameViewHolder> {
    private ArrayList<File> fileList;
    private ClickListener listener;

    public FilenameAdapter(ClickListener listener, ArrayList<File> fileList) {
        this.fileList = fileList;
        this.listener = listener;
    }

    public static class FilenameViewHolder
            extends RecyclerView.ViewHolder implements View.OnClickListener{
        public TextView textView;
        public ClickListener listener;
        public File file;

        public FilenameViewHolder(View v) {
            super(v);

            this.textView = (TextView) v;
            // this.textView = (TextView) v.findViewById(R.id.text);
            v.setOnClickListener(this);
        }

        @Override
        public void onClick(View view) {
            int position = getAdapterPosition();
            if (position != RecyclerView.NO_POSITION) {
                // Toast.makeText(context, textView.getText(), Toast.LENGTH_SHORT).show();
                // listener.itemClicked(textView.getText().toString());
                listener.itemClicked(file);
            }
        }
    }

    // Create new views
    @Override
    public FilenameAdapter.FilenameViewHolder onCreateViewHolder(ViewGroup parent,
            int viewType) {

        View view = (View) LayoutInflater.from(parent.getContext())
                .inflate(R.layout.filename_recycler_item, parent, false);

        FilenameViewHolder vh = new FilenameViewHolder(view);
        vh.listener = listener;
        return vh;
    }

    // Replace view text
    @Override
    public void onBindViewHolder(FilenameViewHolder holder, int position) {
        holder.textView.setText(fileList.get(position).getName());
        holder.file = fileList.get(position);
    }

    @Override
    public int getItemCount() {
        return fileList.size();
    }
}

