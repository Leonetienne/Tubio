<template>
    <div class="download-box">
        <h2 v-if="false" class="no-dls-yet mt-2">No downloads yet...</h2>

        <DownloadEntry v-for="(nObj, nIdx) in dlcache" :downloadEntry="nObj" :key="nIdx" />

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
    dlcache: function() {
      return this.$store.state.dlcache.cache;
    }
  },

  data() {
    return {
      downloads: {type: Array}
    };
  },

  mounted() {
    const that = this;
    this.$store.dispatch("dlcache/update", this);
    setInterval(function(){
      that.$store.dispatch("dlcache/update", that);
    }, 1000);
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