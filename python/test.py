# import sys
# sys.path.append('./bin/')

import alm

txt = ""

class Callback(alm.callback_t):
    def status(self, status):
        print('status:', status)
    
    def text(self, text):
        global txt
        print(text)
        txt = text

    def tokens(self, word, context, reset, stop):
        print(word, " => ", context)
        return True
    
    def sentences(self, text):
        print("Sentences:", text)
        return True

a = alm.lm_t("abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюяé")

callback = Callback()

a.setCallback(callback)

# a.setOption(alm.options_t_debug)
a.setOption(alm.options_t_mixdicts)
# a.setOption(alm.options_t_onlyGood)
a.setOption(alm.options_t_confidence)

a.switchAllowApostrophe()

a.read('../lm.arpa')

a.tokenization("«On nous dit qu'aujourd'hui c'est le cas, encore faudra-t-il l'évaluer» l'astronomie")

a.textToJson("«On nous dit qu'aujourd'hui c'est le cas, encore faudra-t-il l'évaluer» l'astronomie")

a.jsonToText(txt)

a.find("Особое место занимает чудотворная икона Лобзание Христа Иудою")

a.sentences()

ppl = a.perplexity("неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика....")

print(ppl.logprob)

print(a.idt("<unk>"))
print(a.idw("приветик"))

print(a.roman2Arabic("vi"))

print(a.fti(53.89894, 5))
