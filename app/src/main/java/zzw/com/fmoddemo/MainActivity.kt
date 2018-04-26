package zzw.com.fmoddemo

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*
import org.fmod.FMOD
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors


class MainActivity : AppCompatActivity(), View.OnClickListener {

    private var fixedThreadPool: ExecutorService? = null
    private var playerThread: PlayerThread? = null
    private val path = "file:///android_asset/music.mp3"
    private var type: Int = 0


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        normal!!.setOnClickListener(this)
        luoli!!.setOnClickListener(this)
        dashu!!.setOnClickListener(this)
        jingsong!!.setOnClickListener(this)
        gaoguai!!.setOnClickListener(this)
        kongling!!.setOnClickListener(this)

        fixedThreadPool = Executors.newFixedThreadPool(1)
        FMOD.init(this)
    }

    internal inner class PlayerThread : Runnable {
        override fun run() {
            VoiceHelper.fix(path, type)
        }
    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.normal -> type = VoiceHelper.MODE_NORMAL
            R.id.luoli -> type = VoiceHelper.MODE_LUOLI
            R.id.dashu -> type = VoiceHelper.MODE_DASHU
            R.id.jingsong -> type = VoiceHelper.MODE_JINGSONG
            R.id.gaoguai -> type = VoiceHelper.MODE_GAOGUAI
            R.id.kongling -> type = VoiceHelper.MODE_KONGLING
        }

        playerThread = PlayerThread()
        fixedThreadPool!!.execute(playerThread!!)
    }

    override fun onDestroy() {
        super.onDestroy()
        FMOD.close()
    }
}
