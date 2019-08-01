package ar.com.mbernardi.sdl_fungeoid;

import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.LinearLayoutManager;
import android.content.Intent;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.TextView;
import android.net.Uri;
import java.util.ArrayList;
import java.io.File;

import ar.com.mbernardi.sdl_fungeoid.FileActivity;

class FilenameAdapter extends RecyclerView.Adapter<FilenameAdapter.FilenameViewHolder> {
    private ArrayList<File> fileList;
    private FileActivity listener;

    public FilenameAdapter(FileActivity listener, ArrayList<File> fileList) {
        this.fileList = fileList;
        this.listener = listener;
    }

    public static class FilenameViewHolder
            extends RecyclerView.ViewHolder implements View.OnClickListener{
        public TextView textView;
        public FileActivity listener;
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

