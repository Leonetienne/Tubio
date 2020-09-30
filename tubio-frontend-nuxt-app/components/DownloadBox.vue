<template>
    <div class="download-box">
        <h2 v-if="false" class="no-dls-yet mt-2">No downloads yet...</h2>

        <DownloadEntry v-for="(nObj, nIdx) in downloads_c" :downloadEntry="nObj" :key="nIdx" />

    </div>
</template>

<script>
import DownloadEntry from "~/components/DownloadEntry";
import axios from "axios";

export default {
  components: {
      DownloadEntry,
  },

  computed: {
    downloads_c: function() {
      return this.downloads;
    }
  },

  data() {
    return {
      downloads: {type: Array}
    };
  },

  mounted() {
    const that = this;
    axios.get("/rest-dummies/cache.json", {
      responseType: 'text'
    }).then(function(response){
      if (response.data.status === "OK") {
        console.log(response.data);
        that.downloads = response.data.cache;
      }
    });

    return;
  }
  
}
</script>

<style lang="scss" scoped>
.download-box {
  width: 100%;
  min-height: 600px;
  border-radius: 5px;
  padding: 20px;
  background-color: #fff5;
}

.no-dls-yet {
  color: theme("colors.text-gray-1");
  font-size: 34pt;
  text-align: center;
}
</style>