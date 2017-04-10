using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ImageInDb
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "Choose Image";
            openFileDialog1.Filter = "Images (*.JPEG;*.BMP;*.JPG;*.GIF;*.PNG;*.)|*.JPEG;*.BMP;*.JPG;*.GIF;*.PNG";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Image img = new Bitmap(openFileDialog1.FileName);
                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
                pictureBox1.Image = img;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] byteImg = ImageToByteArray(pictureBox1.Image, pictureBox1);
            string im = System.Text.Encoding.ASCII.GetString(byteImg);
            System.Data.SqlClient.SqlConnection con = new System.Data.SqlClient.SqlConnection("Data Source=ppwpdsql02\\sqldv2k8r2;Initial Catalog=LNTBPM;User ID=wfdbuser;Password=wfdbpassword");
            string sql = "update Test set image = @param where id = 1";
            System.Data.SqlClient.SqlCommand cmd = new System.Data.SqlClient.SqlCommand(sql, con);
            cmd.Parameters.AddWithValue("@param", byteImg);
            con.Open();
            int i = cmd.ExecuteNonQuery();
            pictureBox1.Image = null;
        }

        public static byte[] ImageToByteArray(Image img, PictureBox pictureBox1)
        {
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            if (pictureBox1.Image != null)
            {
                img.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            }
            return ms.ToArray();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            byte[] gotImage=null;
            System.Data.SqlClient.SqlConnection con = new System.Data.SqlClient.SqlConnection("Data Source=ppwpdsql02\\sqldv2k8r2;Initial Catalog=LNTBPM;User ID=wfdbuser;Password=wfdbpassword");
            string sql = "select image from Test where id = 1";
            System.Data.SqlClient.SqlCommand cmd = new System.Data.SqlClient.SqlCommand(sql, con);
            con.Open();
            System.Data.SqlClient.SqlDataReader sdr = cmd.ExecuteReader();
            if (sdr.HasRows)
            {
                while (sdr.Read())
                {
                    gotImage = (byte[])sdr["image"];
                }
            }
            pictureBox1.Image = GetDataToImage(gotImage);
        }

        public Image GetDataToImage(byte[] pData)
        {
            try
            {
                ImageConverter imgConverter = new ImageConverter();
                return imgConverter.ConvertFrom(pData) as Image;
            }
            catch (Exception ex)
            {
                //MsgBox.Show(ex.Message, "Error", MsgBox.Buttons.OK, MsgBox.Icon.Error);
                return null;
            }
        }
    }
}
